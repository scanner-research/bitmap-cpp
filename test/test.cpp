#include "bitmap-cpp/bitmap.h"
#include <fstream>
#include <sstream>
#include <assert.h>
#include <iostream>
#include <chrono>

using namespace std::chrono;
typedef high_resolution_clock t;

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
  auto start = t::now();
  bitmap::DecodeResult result = bitmap::bitmap_decode(input, size, output);
  auto duration = duration_cast<nanoseconds>(t::now() - start).count();
  printf("%.3fms\n", duration / 1000000.);
  assert(result == bitmap::DecodeResult::Success);
}
