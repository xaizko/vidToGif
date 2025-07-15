CC = gcc
OUTPUT = -o vidToGif
SRC = vidToGif.c
LIBS = -lavformat -lavcodec -lavutil -lswscale -lm -lz

all: 
	$(CC) $(SRC) $(OUTPUT) $(LIBS)

clean: 
	rm -f vidToGif

.PHONY: all clean

