#include "Halide.h"

namespace bitmap {

using namespace Halide;

class BitmapTransformer : public Generator<BitmapTransformer> {
public:
  ImageParam input{UInt(8), 3, "input"};
  Param<int32_t> width{"width"};
  Param<int32_t> height{"height"};

  Func build() {
    Var x("x"), y("y"), c("c");

    Func output("output");
    output(x, y, c) = input(x, y, c);

    input
      .dim(0).set_stride(3)
      .dim(2).set_stride(1)
      .dim(2).set_bounds(0, 3);

    output.output_buffer()
      .dim(0).set_stride(3)
      .dim(2).set_stride(1)
      .dim(2).set_bounds(0, 3);

    output
      .estimate(x, 0, width)
      .estimate(y, 0, width)
      .estimate(c, 0, 3);

    Target target = Halide::get_target_from_environment();
    Pipeline p(output);
    p.auto_schedule(target);

    return output;
  }
};

RegisterGenerator<BitmapTransformer> register_me{"bitmap_transformer"};

}
