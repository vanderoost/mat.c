#include "helpers.h"
#include "mat/mat.h"
#include <assert.h>

float one(void) { return 1.0f; }

void test_mat_fun_ones(void) {
  // clang-format off
  Mat dst = mat_new(2, 2, (float[4]) {0});
  Mat exp = mat_new(2, 2, (float[4]) {
    1, 1,
    1, 1,
  });
  // clang-format on

  mat_gen(dst, one);

  assert(mat_eq(dst, exp));
}

int main(void) {
  test_mat_fun_ones();

  return 0;
}
