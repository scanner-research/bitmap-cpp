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
      .set_stride(0, 3)
      .set_stride(2, 1);

    Target target = Halide::get_target_from_environment();
    Pipeline p(foo);
    p.auto_schedule(target);

    return foo;
  }
};

RegisterGenerator<BitmapTransformer> register_me{"bitmap_transformer"};

}
