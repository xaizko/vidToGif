CC = gcc
OUTPUT = -o vidToGif
SRC = vidToGif.c

all: 
	$(CC) $(SRC) $(OUTPUT)

clean: 
	rm -f vidToGif

.PHONY: all clean

