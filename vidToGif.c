#include "vidToGif.h"

int main(int argc, char *argv[]) {
    
    //check for correct amount of arguments
    if (argc < 3) {
	fprintf(stderr, "Usage: %s <path to video> <desired output path>\n", argv[0]);
	return -1;
    }

    const char *input_path = argv[1];
    const char *output_path = argv[2];

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
}
