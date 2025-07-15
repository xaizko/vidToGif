#include <stdio.h>
#include <stdlib.h>

//ffmpeg libraries
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>

//function declarations
void save_frame_as_gif(AVFrame *pFrameRGB, const int width, const int height, const int frameIndex, const char *output_path);
void cleanup_temp_files(void);
void print_usage(const char *program_name);
