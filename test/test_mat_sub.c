#include "helpers.h"
#include "mat/mat.h"
#include <assert.h>

void test_mat_sub_2x2(void) {
  // clang-format off
  Mat a = mat_new(2, 2, (float[]) {
    8, 8,
    8, 8,
  });
  Mat b = mat_new(2, 2, (float[]) {
    1, 2,
    3, 4,
  });
  Mat dst = mat_new(2, 2, (float[4]) {0});
  Mat exp = mat_new(2, 2, (float[]) {
    7, 6,
    5, 4,
  });
  // clang-format on

  mat_sub(dst, a, b);

  assert(mat_eq(dst, exp));
}

int main(void) {
  test_mat_sub_2x2();

  return 0;
}
