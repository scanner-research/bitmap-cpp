#include "Halide.h"

namespace bitmap {

using namespace Halide;

class BitmapTransformer : public Generator<BitmapTransformer> {
public:
  ImageParam input{UInt(8), 3, "input"};

  Func build() {
    Var x("x"), y("y"), c("c");

    Func foo("foo");
    foo(x, y, c) = input(x, y, c);

    input
      .dim(0).set_stride(3)
      .dim(2).set_stride(1)
      .dim(2).set_bounds(0, 3);

    foo.output_buffer()
      .dim(0).set_stride(3)
      .dim(2).set_stride(1)
      .dim(2).set_bounds(0, 3);

    Target target = Halide::get_target_from_environment();
    Pipeline p(foo);
    p.auto_schedule(target);

    return foo;
  }
};

RegisterGenerator<BitmapTransformer> register_me{"bitmap_transformer"};

}
