#ifndef _MAT_H
#define _MAT_H

#include <stdint.h>

typedef struct {
  uint32_t rows, cols;
  float *data;
} Mat;

void mat_set(Mat dst, float val);
void mat_cpy(Mat dst, Mat src);
void mat_gen(Mat dst, float f(void));
void mat_fun(Mat dst, Mat src, float f(float));
void mat_mul(Mat dst, Mat a, Mat b);

void mat_print(Mat m);

#endif // _MAT_H
