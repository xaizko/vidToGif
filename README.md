# Video to GIF Converter

A simple C program that converts video files to animated GIF files using FFmpeg libraries.

## Requirements

- FFmpeg development libraries (`libavformat`, `libavcodec`, `libavutil`, `libswscale`)
- FFmpeg binary (for final GIF conversion)
- GCC compiler

## Installation

On linux systems:
```bash
sudo [package manager] install libavformat-dev libavcodec-dev libavutil-dev libswscale-dev ffmpeg
```

On macOS (with Homebrew):
```bash
brew install ffmpeg
```

## Building

```bash
make
```

## Usage

```bash
./vidToGif <input_video_file> <output_gif_file>
```

Example:
```bash
./vidToGif video.mp4 output.gif
```

## Features

- Converts any video format supported by FFmpeg to GIF
- Automatically scales output to reasonable size (max 320px width)
- Limits to 100 frames to avoid huge file sizes
- Shows progress during conversion
- Handles cleanup of temporary files

## How it works

1. Uses FFmpeg libraries to decode video frames
2. Converts frames to RGB24 format
3. Saves frames as temporary PPM files
4. Uses FFmpeg binary to convert PPM sequence to animated GIF
5. Cleans up temporary files

## Notes

- Output GIF is limited to 100 frames to keep file size reasonable
- Frame rate is set to 10 FPS for smooth animation
- Large videos will be scaled down to 320px width while maintaining aspect ratio
- This was designed for linux systems, I do not know if it works on windows 
