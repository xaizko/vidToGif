#include "vidToGif.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
	printf("Usage: %s <path/to/input> <path/to/output/\n", argv[0]);
    }

    if (!is_supported_file(argv[1])) {
	printf("Unsupported file type: %s\nPlease input a .mov or .mp4\n", argv[1]);
    }

    if (!is_supported_output(argv[2])) {
	printf("Unsupported output type: %s\n", argv[2]);
	printf("Output must end with .gif\n");
    }
}

bool is_supported_file(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot) return false;

    return strcmp(dot, ".mp4") == 0 || strcmp(dot, ".mov") == 0;
}

bool is_supported_output(const char *output) {
    const char *dot = strrchr(output, '.');
    if (!dot) return false;

    return strcmp(dot, ".gif") == 0;
}
