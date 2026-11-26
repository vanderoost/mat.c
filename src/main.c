#include "mat/mat.h"
#include "rng/rng.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAT_NEW(rows, cols) mat_new((rows), (cols), (float[(rows) * (cols)]){})

float sigmoid(float x) { return 1.0f / (1.0f + expf(-x)); }
void sigmoid_backwards(Mat z, Mat a, Mat da) {
  for (size_t i = 0; i < z.rows; ++i) {
    for (size_t j = 0; j < z.cols; ++j) {
      float a_ij = a.data[i * a.rstr + j * a.cstr];
      float da_ij = da.data[i * da.rstr + j * da.cstr];
      z.data[i * z.rstr + j * z.cstr] = da_ij * a_ij * (1.0f - a_ij);
    }
  }
}

float square_loss(Mat a, Mat y) {
  assert(a.rows == y.rows);
  assert(a.cols == y.cols);

  float result = 0.0f;

  for (size_t i = 0; i < a.rows * a.cols; ++i) {
    float diff = a.data[i] - y.data[i];
    result += diff * diff;
  }

  return result;
}
float log_loss(Mat a, Mat y) {
  assert(a.rows == y.rows);
  assert(a.cols == y.cols);

  float result = 0.0f;

  for (size_t i = 0; i < a.rows * a.cols; ++i) {
    float ai = a.data[i];
    float yi = y.data[i];
    result += -yi * logf(ai) - (1.0f - yi) * logf(1.0f - ai);
  }

  return result;
}

int main(void) {
  rng_seed(1, 1);
  puts("XOR Neural Network");

  // DEFINE NETWORK ARCHITECTURE
#define LAY_0 2 // Input
#define LAY_1 3 // Hidden
#define LAY_2 1 // Output
#define BATCH_LEN 4
#define BIAS 1

  // TRAINING DATA
  // clang-format off
  Mat xs = mat_new(BATCH_LEN, LAY_0, (float[]) {
    0, 0,
    0, 1,
    1, 0,
    1, 1,
  });
  Mat ys = mat_new(BATCH_LEN, LAY_2, (float[]) {
    0,
    1,
    1,
    0,
  });
  // clang-format on

  // CREATE MATRICES
  Mat a0 = MAT_NEW(BATCH_LEN, LAY_0 + BIAS);
  Mat w0 = MAT_NEW(LAY_0 + BIAS, LAY_1);
  Mat dw0 = MAT_NEW(LAY_0 + BIAS, LAY_1);
  Mat ew0 = MAT_NEW(LAY_0 + BIAS, LAY_1);
  Mat z0 = MAT_NEW(BATCH_LEN, LAY_1);
  Mat dz0 = MAT_NEW(BATCH_LEN, LAY_1);

  Mat a1 = MAT_NEW(BATCH_LEN, LAY_1 + BIAS);
  Mat da1 = MAT_NEW(BATCH_LEN, LAY_1);
  Mat w1 = MAT_NEW(LAY_1 + BIAS, LAY_2);
  Mat dw1 = MAT_NEW(LAY_1 + BIAS, LAY_2);
  Mat ew1 = MAT_NEW(LAY_1 + BIAS, LAY_2);
  Mat z1 = MAT_NEW(BATCH_LEN, LAY_2);
  Mat dz1 = MAT_NEW(BATCH_LEN, LAY_2);

  Mat a2 = MAT_NEW(BATCH_LEN, LAY_2);

  // RANDOM INIT WEIGHTS
  mat_gen(w0, rng_norm);
  mat_gen(w1, rng_norm);

  // SET BIAS NEURONS
  mat_set(a0, 1.0f);
  mat_set(a1, 1.0f);

  // SET THE INPUT
  mat_cpy(a0, xs);

#define EPOCHS 4000
#define PRINTS 10

  float alpha = 1e-1;
  float eps = 1e-4;
  for (size_t epoch = 0; epoch < EPOCHS; ++epoch) {

    // FORWARD PASS
    mat_mul(z0, a0, w0);
    mat_fun(a1, z0, sigmoid);
    mat_mul(z1, a1, w1);
    mat_fun(a2, z1, sigmoid);
    float loss = log_loss(a2, ys);
    if (PRINTS > EPOCHS || epoch % (EPOCHS / PRINTS) == 0) {
      printf("loss: %.3f\n", loss);
    }

    // BACKPROP
    mat_sub(dz1, a2, ys);
    mat_mul(dw1, mat_transp(a1), dz1);
    w1.rows -= BIAS;
    mat_mul(da1, dz1, mat_transp(w1));
    sigmoid_backwards(dz0, a1, da1);
    mat_mul(dw0, mat_transp(a0), dz0);
    w1.rows += BIAS;

    // NUMERICAL GRADIENT ESTIMATES
    for (size_t i = 0; i < w0.rows * w0.cols; ++i) {
      float orig_w = w0.data[i];

      w0.data[i] += eps;
      mat_mul(z0, a0, w0);
      mat_fun(a1, z0, sigmoid);
      mat_mul(z1, a1, w1);
      mat_fun(a2, z1, sigmoid);
      float new_loss = log_loss(a2, ys);

      ew0.data[i] = (new_loss - loss) / eps; // Gradient

      w0.data[i] = orig_w;
    }
    for (size_t i = 0; i < w1.rows * w1.cols; ++i) {
      float orig_w = w1.data[i];

      w1.data[i] += eps;
      mat_mul(z0, a0, w0);
      mat_fun(a1, z0, sigmoid);
      mat_mul(z1, a1, w1);
      mat_fun(a2, z1, sigmoid);
      float new_loss = log_loss(a2, ys);

      ew1.data[i] = (new_loss - loss) / eps; // Gradient

      w1.data[i] = orig_w;
    }

    // DEBUG
    // printf("w0 gradients\n");
    // mat_print(dw0);
    // mat_print(ew0);
    // printf("w1 gradients\n");
    // mat_print(dw1);
    // mat_print(ew1);

    // GRADIENT DESCENT (update the weights)
    for (size_t i = 0; i < w0.rows * w0.cols; ++i) {
      w0.data[i] -= alpha * dw0.data[i];
    }
    for (size_t i = 0; i < w1.rows * w1.cols; ++i) {
      w1.data[i] -= alpha * dw1.data[i];
    }
  }

  // DEBUG
  printf("output:\n");
  mat_print(a2);
  printf("target:\n");
  mat_print(ys);

  return 0;
}
