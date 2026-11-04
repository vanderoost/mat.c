#include "mat.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

void mat_set(Mat dst, float val) {
  for (size_t i = 0; i < dst.rows * dst.cols; ++i) {
    dst.data[i] = val;
  }
}

void mat_cpy(Mat dst, Mat src) {
  assert(dst.rows >= src.rows);
  assert(dst.cols >= src.cols);

  for (size_t i = 0; i < src.rows; ++i) {
    for (size_t j = 0; j < src.cols; ++j) {
      dst.data[i * dst.cols + j] = src.data[i * src.cols + j];
    }
  }
}

void mat_gen(Mat dst, float f(void)) {
  for (size_t i = 0; i < dst.rows * dst.cols; ++i) {
    dst.data[i] = f();
  }
}

void mat_fun(Mat dst, Mat src, float f(float)) {
  assert(dst.rows >= src.rows);
  assert(dst.cols >= src.cols);

  for (size_t i = 0; i < src.rows; ++i) {
    for (size_t j = 0; j < src.cols; ++j) {
      dst.data[i * dst.cols + j] = f(src.data[i * src.cols + j]);
    }
  }
}

void mat_mul(Mat dst, Mat a, Mat b) {
  memset(dst.data, 0, dst.rows * dst.cols * sizeof *dst.data);

  for (size_t i = 0; i < dst.rows; ++i) {
    for (size_t k = 0; k < a.cols; ++k) {
      for (size_t j = 0; j < dst.cols; ++j) {
        dst.data[i * dst.cols + j] += a.data[i * a.cols + k] * b.data[k * b.cols + j];
      }
    }
  }
}

void mat_print(Mat m) {
  for (size_t i = 0; i < m.rows; ++i) {
    for (size_t j = 0; j < m.cols; ++j) {
      printf(" %6.2f", m.data[i * m.cols + j]);
    }
    printf("\n");
  }
}
