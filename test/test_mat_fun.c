#include "helpers.h"
#include "mat/mat.h"
#include <assert.h>

float add_one(float x) { return x + 1.0f; }

void test_mat_fun_add_one(void) {
  // clang-format off
  Mat src = mat_new(2, 3, (float[]) {
    1, 2, 3,
    4, 5, 6,
  });
  Mat dst = mat_new(2, 3, (float[2 * 3]) {0});
  Mat exp = mat_new(2, 3, (float[]) {
    2, 3, 4,
    5, 6, 7,
  });
  // clang-format on

  mat_fun(dst, src, add_one);

  assert(mat_eq(dst, exp));
}

int main(void) {
  test_mat_fun_add_one();

  return 0;
}
