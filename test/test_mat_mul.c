#include "helpers.h"
#include "mat/mat.h"
#include <assert.h>

void test_mat_mul_square(void) {
  // clang-format off
  Mat a = {2, 2, (float[]) {
    1, 2,
    3, 4,
  }};
  Mat b = {2, 2, (float[]) {
    4, 3,
    2, 1,
  }};
  Mat dst = {2, 2, (float[4]) {0}};
  Mat exp = {2, 2, (float[]) {
    8,    5,
    20,   13,
  }};
  // clang-format on

  mat_mul(dst, a, b);

  assert(mat_eq(dst, exp));
}

void test_mat_mul_vec(void) {
  // clang-format off
  Mat a = {3, 1, (float[]) {
    3,
    2,
    1,
  }};
  Mat b = {1, 4, (float[]) {
    1, 2, 3, 4,
  }};
  Mat dst = {3, 4, (float[3 * 4]) {0}};
  Mat exp = {3, 4, (float[]) {
    3,    6,    9,   12,
    2,    4,    6,    8,
    1,    2,    3,    4,
  }};
  // clang-format on

  mat_mul(dst, a, b);

  assert(mat_eq(dst, exp));
}

int main(void) {
  test_mat_mul_square();
  test_mat_mul_vec();

  return 0;
}
