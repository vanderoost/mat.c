#include "mat/mat.h"
#include "rng/rng.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAT_SIZE 512
#define BEST_OF_N 8

int main(void) {
  float *mat_data = malloc(MAT_SIZE * MAT_SIZE * 3 * sizeof(*mat_data));

  Mat a = {
      .rows = MAT_SIZE,
      .cols = MAT_SIZE,
      .data = mat_data + 0 * MAT_SIZE * MAT_SIZE,

  };
  Mat b = {
      .rows = MAT_SIZE,
      .cols = MAT_SIZE,
      .data = mat_data + 1 * MAT_SIZE * MAT_SIZE,
  };
  Mat c = {
      .rows = MAT_SIZE,
      .cols = MAT_SIZE,
      .data = mat_data + 2 * MAT_SIZE * MAT_SIZE,
  };

  mat_fun(a, rng_norm);
  mat_fun(b, rng_norm);

  uint64_t fastest_ns = UINT64_MAX;
  for (size_t i = 0; i < BEST_OF_N; ++i) {
    uint64_t start_ns = clock_gettime_nsec_np(CLOCK_MONOTONIC);
    mat_mul(c, a, b);
    uint64_t dur_ns = clock_gettime_nsec_np(CLOCK_MONOTONIC) - start_ns;
    if (dur_ns < fastest_ns) {
      fastest_ns = dur_ns;
    }
  }
  double dur_s = fastest_ns / 1e9;
  printf("mat_mul time: %.3f s\n", dur_s);

  free(mat_data);

  return 0;
}
