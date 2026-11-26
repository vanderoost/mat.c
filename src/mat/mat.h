#ifndef _MAT_H
#define _MAT_H

#include <stdint.h>

typedef struct {
  uint32_t rows, cols;
  uint32_t rstr, cstr;
  float *data;
} Mat;

Mat mat_new(uint32_t rows, uint32_t cols, float *data);
void mat_set(Mat dst, float val);
void mat_cpy(Mat dst, Mat src);
Mat mat_transp(Mat m);
void mat_gen(Mat dst, float f(void));
void mat_fun(Mat dst, Mat src, float f(float));
void mat_sub(Mat dst, Mat a, Mat b);
void mat_mul(Mat dst, Mat a, Mat b);

void mat_print(Mat m);

#endif // _MAT_H
