#include "bitmap-cpp/bitmap.h"
#include <fstream>
#include <sstream>
#include <assert.h>

int main() {
  std::ifstream input_file("input.bmp", std::ios::binary);
  std::stringstream sstr;
  sstr << input_file.rdbuf();
  std::string bytes = sstr.str();
  const uint8_t* input = reinterpret_cast<const uint8_t*>(bytes.c_str());
  size_t size = bytes.size();

  bitmap::BitmapMetadata metadata;
  bitmap::bitmap_metadata(input, size, metadata);

  unsigned char* output = new uint8_t[metadata.width * metadata.height * 3];
  bitmap::DecodeResult result = bitmap::bitmap_decode(input, size, output);
  assert(result == bitmap::DecodeResult::Success);
}
