#include "vidToGif.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
	printf("Usage: %s <path/to/input> <path/to/output/\n", argv[0]);
    }
}

bool is_supported_file(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot) return false;

    return strcmp(dot, ".mp4") == 0 || strcmp(dot, ".mov") == 0;
}
