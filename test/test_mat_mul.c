#include "helpers.h"
#include "mat/mat.h"
#include <assert.h>

void test_mat_mul_square(void) {
  // clang-format off
  Mat a = mat_new(2, 2, (float[]) {
    1, 2,
    3, 4,
  });
  Mat b = mat_new(2, 2, (float[]) {
    4, 3,
    2, 1,
  });
  Mat dst = mat_new(2, 2, (float[4]) {0});
  Mat exp = mat_new(2, 2, (float[]) {
    8,    5,
    20,   13,
  });
  // clang-format on

  mat_mul(dst, a, b);

  assert(mat_eq(dst, exp));
}

void test_mat_mul_vec(void) {
  // clang-format off
  Mat a = mat_new(3, 1, (float[]) {
    3,
    2,
    1,
  });
  Mat b = mat_new(1, 4, (float[]) {
    1, 2, 3, 4,
  });
  Mat dst = mat_new(3, 4, (float[3 * 4]) {0});
  Mat exp = mat_new(3, 4, (float[]) {
    3,    6,    9,   12,
    2,    4,    6,    8,
    1,    2,    3,    4,
  });
  // clang-format on

  mat_mul(dst, a, b);

  assert(mat_eq(dst, exp));
}

int main(void) {
  test_mat_mul_square();
  test_mat_mul_vec();

  return 0;
}
