#include "bitmap.h"
#include <stdio.h>

namespace bitmap {

static const unsigned MINHEADER = 54; //minimum BMP header size

DecodeResult bitmap_metadata(const unsigned char* bmp, size_t size,
                             BitmapMetadata& metadata) {
  if(size < MINHEADER) {
    return DecodeResult::Invalid;
  }
  if(bmp[0] != 'B' || bmp[1] != 'M') {
    return DecodeResult::Invalid;
  } //It's not a BMP file if it doesn't start with marker 'BM'
  unsigned pixeloffset = bmp[10] + 256 * bmp[11]; //where the pixel data starts
  //read width and height from BMP header
  metadata.width = bmp[18] + bmp[19] * 256;
  metadata.height = bmp[22] + bmp[23] * 256;
  //read number of channels from BMP header
  if(bmp[28] != 24 && bmp[28] != 32) {
    return DecodeResult::UnsupportedBitmap; //only 24-bit and 32-bit BMPs are supported.
  }
  unsigned numChannels = bmp[28] / 8;

  if (numChannels == 3) {
    metadata.color_space = ImageColorSpace::RGB;
  } else if (numChannels == 4) {
    metadata.color_space = ImageColorSpace::RGBA;
  } else {
    return DecodeResult::UnsupportedBitmap;
  }

  return DecodeResult::Success;
}

DecodeResult bitmap_decode(const unsigned char* bmp, size_t size,
                           unsigned char* output) {
  BitmapMetadata metadata;
  DecodeResult result = bitmap_metadata(bmp, size, metadata);
  if (result != DecodeResult::Success) {
    return result;
  }

  unsigned pixeloffset = bmp[10] + 256 * bmp[11]; //where the pixel data starts
  unsigned numChannels = metadata.color_space == ImageColorSpace::RGB ? 3 : 4;
  unsigned w = metadata.width;
  unsigned h = metadata.height;

  //The amount of scanline bytes is width of image times channels, with extra bytes added if needed
  //to make it a multiple of 4 bytes.
  unsigned scanlineBytes = w * numChannels;
  if(scanlineBytes % 4 != 0) scanlineBytes = (scanlineBytes / 4) * 4 + 4;

  unsigned dataSize = scanlineBytes * h;
  if(size < dataSize + pixeloffset) {
    return DecodeResult::Invalid;//BMP file too small to contain all pixels
  }

  /*
  There are 3 differences between BMP and the raw image buffer for LodePNG:
  -it's upside down
  -it's in BGR instead of RGB format (or BRGA instead of RGBA)
  -each scanline has padding bytes to make it a multiple of 4 if needed
  The 2D for loop below does all these 3 conversions at once.
  */
  for(unsigned y = 0; y < h; y++) {
    for(unsigned x = 0; x < w; x++) {
      //pixel start byte position in the BMP
      unsigned bmpos = pixeloffset + (h - y - 1) * scanlineBytes + numChannels * x;
      //pixel start byte position in the new raw image
      unsigned newpos = 4 * y * w + 4 * x;
      if(numChannels == 3)
      {
        output[newpos + 0] = bmp[bmpos + 2]; //R
        output[newpos + 1] = bmp[bmpos + 1]; //G
        output[newpos + 2] = bmp[bmpos + 0]; //B
        output[newpos + 3] = 255;            //A
      }
      else
      {
        output[newpos + 0] = bmp[bmpos + 3]; //R
        output[newpos + 1] = bmp[bmpos + 2]; //G
        output[newpos + 2] = bmp[bmpos + 1]; //B
        output[newpos + 3] = bmp[bmpos + 0]; //A
      }
    }
  }

  return DecodeResult::Success;
}

}
