#pragma once

#include <vector>
#include <stdlib.h>
#include <stdint.h>

namespace bitmap {

// https://raw.githubusercontent.com/lvandeve/lodepng/master/examples/example_bmp2png.cpp

enum ImageColorSpace {
  RGB,
  RGBA
};

struct BitmapMetadata {
  int32_t width;
  int32_t height;
  ImageColorSpace color_space;
};

enum DecodeResult {
  Success,
  Invalid,
  UnsupportedBitmap
};

DecodeResult bitmap_metadata(const unsigned char* bmp, size_t size,
                             BitmapMetadata& metadata);
DecodeResult bitmap_decode(const unsigned char* bmp, size_t size,
                           unsigned char* output);

}
