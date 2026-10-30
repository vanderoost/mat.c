#include "mat/mat.h"
#include "rng/rng.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define MAT_SIZE 512
#define BEST_OF_N 8

// NOTE(Giovanni): cross-platform precise time function :)
static uint64_t get_time_ns(void) {
#ifdef _WIN32
  LARGE_INTEGER freq, counter;
  QueryPerformanceFrequency(&freq);
  QueryPerformanceCounter(&counter);
  return (uint64_t)(((double)counter.QuadPart / (double)freq.QuadPart) * 1e9);
#elif defined(__APPLE__)
  return clock_gettime_nsec_np(CLOCK_MONOTONIC);
#else
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
#endif
}

int main(void) {
  float *mat_data = malloc(MAT_SIZE * MAT_SIZE * 3 * sizeof(*mat_data));
  if (mat_data == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    return 1;
  }

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
    uint64_t start_ns = get_time_ns();
    mat_mul(c, a, b);
    uint64_t dur_ns = get_time_ns() - start_ns;
    if (dur_ns < fastest_ns) {
      fastest_ns = dur_ns;
    }
  }
  double dur_s = (double)fastest_ns / 1e9;
  printf("mat_mul time: %.3f s\n", dur_s);

  free(mat_data);

  return 0;
}
