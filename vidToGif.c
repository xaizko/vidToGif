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
}
