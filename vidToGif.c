#include "vidToGif.h"

int main(int argc, char *argv[]) {
    
    //check for correct amount of arguments
    if (argc < 3) {
	print_usage(argv[0]);
	return -1;
    }

    const char *input_path = argv[1];
    const char *output_path = argv[2];
    int maxFrames = 100; //default max frames
    
    //check for optional max frames argument
    if (argc >= 4) {
        maxFrames = atoi(argv[3]);
        if (maxFrames <= 0) {
            fprintf(stderr, "max_frames must be a positive number\n");
            return -1;
        }
    }

    //initialize ffmpeg (deprecated in newer versions but still needed for older ones)
    #if LIBAVFORMAT_VERSION_INT < AV_VERSION_INT(58, 29, 100)
    av_register_all();
    #endif

    //get format context
    AVFormatContext *format_ctx = NULL;
    if (avformat_open_input(&format_ctx, input_path, NULL, NULL) != 0) {
	fprintf(stderr, "Could not open input file (%s)\n", input_path);
	return -1;
    }

    //get stream info 
    if (avformat_find_stream_info(format_ctx, NULL) < 0) {
	fprintf(stderr, "Failed to get stream info\n");
	return -1;
    }

    //find first video stream
    int video_stream_index = -1;
    for (int i = 0; i < format_ctx->nb_streams; i++) {
	if (format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
	    video_stream_index = i;
	    break;
	}
    }

    if (video_stream_index == -1) {
	fprintf(stderr, "Failed to find video stream\n");
	return -1;
    }

    //get codec parameters
    AVCodecParameters *codecpar = format_ctx->streams[video_stream_index]->codecpar;
    const AVCodec *codec = avcodec_find_decoder(codecpar->codec_id);
    if (!codec) {
	fprintf(stderr, "Unsupported codec\n");
	return -1;
    }

    //set up codec context
    AVCodecContext *codecContext = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecContext, codecpar);
    if (avcodec_open2(codecContext, codec, NULL) < 0) {
	fprintf(stderr, "Could not open codec\n");
	return -1;
    }

    //allocate frames
    AVFrame *frame = av_frame_alloc();
    AVFrame *frameRGB = av_frame_alloc();
    if (!frame || !frameRGB) {
	fprintf(stderr, "Could not allocate frame\n");
	return -1;
    }

    int width = codecContext->width;
    int height = codecContext->height;
    
    printf("video dimensions: %dx%d\n", width, height);
    
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, width, height, 1);
    uint8_t *buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
    if (!buffer) {
        fprintf(stderr, "could not allocate buffer\n");
        return -1;
    }

    av_image_fill_arrays(frameRGB->data, frameRGB->linesize, buffer, AV_PIX_FMT_RGB24, width, height, 1);

    struct SwsContext *sws_ctx = sws_getContext(width, height, codecContext->pix_fmt, width, height, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);
    if (!sws_ctx) {
        fprintf(stderr, "could not create scaling context\n");
        av_free(buffer);
        av_frame_free(&frame);
        av_frame_free(&frameRGB);
        avcodec_free_context(&codecContext);
        avformat_close_input(&format_ctx);
        return -1;
    }

    AVPacket packet;
    int frameIndex = 0;

    printf("extracting frames (max: %d)...\n", maxFrames);
    while (av_read_frame(format_ctx, &packet) >= 0 && frameIndex < maxFrames) {
	if (packet.stream_index == video_stream_index) {
	    if (avcodec_send_packet(codecContext, &packet) == 0) {
		while (avcodec_receive_frame(codecContext, frame) == 0) {
		    sws_scale(sws_ctx, (uint8_t const * const *)frame->data, frame->linesize, 0, height, frameRGB->data, frameRGB->linesize);

		    save_frame_as_gif(frameRGB, width, height, frameIndex++, output_path);
		    
		    //show progress every 10 frames
		    if (frameIndex % 10 == 0) {
		        printf("processed %d frames\n", frameIndex);
		    }
		}
	    }
	}
	av_packet_unref(&packet);
    }

    printf("extracted %d frames total\n", frameIndex);
    
    if (frameIndex == 0) {
        fprintf(stderr, "no frames were extracted from the video\n");
        cleanup_temp_files();
        //clean up 
        sws_freeContext(sws_ctx);
        av_free(buffer);
        av_frame_free(&frame);
        av_frame_free(&frameRGB);
        avcodec_free_context(&codecContext);
        avformat_close_input(&format_ctx);
        return -1;
    }

    //convert ppm frames to gif using system command
    printf("converting frames to gif...\n");
    char convert_cmd[512];
    snprintf(convert_cmd, sizeof(convert_cmd), 
             "ffmpeg -y -framerate 10 -i frame_%%04d.ppm -vf \"fps=10,scale=320:-1:flags=lanczos\" \"%s\"", 
             output_path);
    
    int result = system(convert_cmd);
    if (result == 0) {
        printf("gif created successfully: %s\n", output_path);
        
        //clean up temporary ppm files
        cleanup_temp_files();
    } else {
        fprintf(stderr, "failed to create gif\n");
        cleanup_temp_files();
    }

    //clean up 
    sws_freeContext(sws_ctx);
    av_free(buffer);
    av_frame_free(&frame);
    av_frame_free(&frameRGB);
    avcodec_free_context(&codecContext);
    avformat_close_input(&format_ctx);
}

void save_frame_as_gif(AVFrame *pFrameRGB, const int width, const int height, const int frameIndex, const char *output_path) {
    //create filename for individual frame
    char filename[256];
    snprintf(filename, sizeof(filename), "frame_%04d.ppm", frameIndex);
    
    FILE *pFile = fopen(filename, "wb");
    if (pFile == NULL) return;
    
    //write ppm header
    fprintf(pFile, "P6\n%d %d\n255\n", width, height);
    
    //write pixel data
    for (int y = 0; y < height; y++) {
        fwrite(pFrameRGB->data[0] + y * pFrameRGB->linesize[0], 1, width * 3, pFile);
    }
    
    fclose(pFile);
}

void cleanup_temp_files(void) {
    //remove any temporary ppm files
    system("rm -f frame_*.ppm");
}

void print_usage(const char *program_name) {
    printf("usage: %s <input_video> <output_gif> [max_frames]\n", program_name);
    printf("  input_video  - path to input video file\n");
    printf("  output_gif   - path to output gif file\n");
    printf("  max_frames   - maximum number of frames (default: 100)\n");
}
