#include "mat/mat.h"
#include "rng/rng.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAT_NEW(rows, cols)                                                            \
  {                                                                                    \
    (rows), (cols), (float[(rows) * (cols)]) {}                                        \
  }

float sigmoid(float x) { return 1.0f / (1.0f + expf(-x)); }

float sq_diff(Mat a, Mat b) {
  assert(a.rows == b.rows);
  assert(a.cols == b.cols);

  float result = 0.0f;

  for (size_t i = 0; i < a.rows * a.cols; ++i) {
    float diff = a.data[i] - b.data[i];
    result += diff * diff;
  }

  return result;
}

int main(void) {
  rng_seed(1, 1);
  puts("XOR Neural Network");

  // DEFINE NETWORK ARCHITECTURE
#define LAY_0 2 // Input
#define LAY_1 2 // Hidden
#define LAY_2 1 // Output
#define BATCH_LEN 4
#define BIAS 1

  // TRAINING DATA
  // clang-format off
  Mat xs = {BATCH_LEN, LAY_0, (float[]) {
    0, 0,
    0, 1,
    1, 0,
    1, 1,
  }};
  Mat ys = {BATCH_LEN, LAY_2, (float[]) {
    0,
    1,
    1,
    0,
  }};
  // clang-format on

  // CREATE MATRICES
  Mat a0 = MAT_NEW(BATCH_LEN, LAY_0 + BIAS);
  Mat w0 = MAT_NEW(LAY_0 + BIAS, LAY_1);
  Mat dw0 = MAT_NEW(LAY_0 + BIAS, LAY_1);
  Mat z0 = MAT_NEW(BATCH_LEN, LAY_1);

  Mat a1 = MAT_NEW(BATCH_LEN, LAY_1 + BIAS);
  Mat w1 = MAT_NEW(LAY_1 + BIAS, LAY_2);
  Mat dw1 = MAT_NEW(LAY_1 + BIAS, LAY_2);
  Mat z1 = MAT_NEW(BATCH_LEN, LAY_2);

  Mat a2 = MAT_NEW(BATCH_LEN, LAY_2);

  // RANDOM INIT WEIGHTS
  mat_gen(w0, rng_norm);
  mat_gen(w1, rng_norm);

  // SET BIAS NEURONS
  mat_set(a0, 1.0f);
  mat_set(a1, 1.0f);

  // SET THE INPUT
  mat_cpy(a0, xs);

#define EPOCHS 10000
#define PRINTS 10

  float alpha = 1e-1;
  float eps = 1e-4;
  for (size_t epoch = 0; epoch < EPOCHS; ++epoch) {

    // FORWARD PASS
    mat_mul(z0, a0, w0);
    mat_fun(a1, z0, sigmoid);
    mat_mul(z1, a1, w1);
    mat_fun(a2, z1, sigmoid);
    float loss = sq_diff(a2, ys);
    if (epoch % (EPOCHS / PRINTS) == 0) {
      printf("loss: %.3f\n", loss);
    }

    // NUMERICAL GRADIENTS
    for (size_t i = 0; i < w0.rows * w0.cols; ++i) {
      float orig_w = w0.data[i];

      w0.data[i] += eps;
      mat_mul(z0, a0, w0);
      mat_fun(a1, z0, sigmoid);
      mat_mul(z1, a1, w1);
      mat_fun(a2, z1, sigmoid);
      float new_loss = sq_diff(a2, ys);

      dw0.data[i] = (new_loss - loss) / eps; // Gradient

      w0.data[i] = orig_w;
    }
    for (size_t i = 0; i < w1.rows * w1.cols; ++i) {
      float orig_w = w1.data[i];

      w1.data[i] += eps;
      mat_mul(z0, a0, w0);
      mat_fun(a1, z0, sigmoid);
      mat_mul(z1, a1, w1);
      mat_fun(a2, z1, sigmoid);
      float new_loss = sq_diff(a2, ys);

      dw1.data[i] = (new_loss - loss) / eps; // Gradient

      w1.data[i] = orig_w;
    }

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
