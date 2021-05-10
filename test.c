#include "klee/klee.h"

#include <assert.h>

#define THRESH 0.001
#define SMALLEST_ABS 0.01

void sort(float* in, int n)
{
  for (int i = 0; i < n; i++) {
    for (int j = i; j < n; j++) {
      if (in[i] < in[j] && i < j) {
        float tmp = in[i];
        in[i] = in[j];
        in[j] = tmp;
      }
    }
  }
}

float mse1(float* inputs, int n)
{
  float sum = 0.0;

  for (int i = 0; i < n; i++) {
    sum += inputs[i];
  }

  return sum;
}

float mse2(float* inputs, int n)
{
  sort(inputs, n);
  inputs[0] += 1000;
  return mse1(inputs, n);
}

float fl_abs(float in)
{
  if (in < 0.0) {
    return -in;
  } else {
    return in;
  }
}

int main()
{
  int n = 8;

  float inputs[n];
  float inputs_2[n];

  klee_make_symbolic(inputs, sizeof(inputs), "inputs");
  klee_make_symbolic(inputs_2, sizeof(inputs_2), "inputs_2");

  for (int i = 0; i < n; i++) {
    klee_assume(inputs[i] > SMALLEST_ABS);
    klee_assume(inputs_2[i] > SMALLEST_ABS);

    klee_assume(inputs[i] == inputs_2[i]);
  }

  float m1 = mse1(inputs, n);
  float m2 = mse2(inputs, n);

  klee_assert((m1 - m2) / m1 < THRESH);
}
