#ifndef _MAT_H
#define _MAT_H

#include <stdint.h>

typedef struct {
  uint32_t rows, cols;
  float *data;
} Mat;

void mat_fun(Mat m, float f(void));
void mat_mul(Mat c, Mat a, Mat b);

void mat_print(Mat m);

#endif // _MAT_H
