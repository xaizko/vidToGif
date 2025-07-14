#include "vidToGif.h"

int main(int argc, char *argv[]) {
    
    //check for correct amount of arguments
    if (argc < 3) {
	fprintf(stderr, "Usage: %s <path to video> <desired output path>\n", argv[0]);
	return -1;
    }
}
