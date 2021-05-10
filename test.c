#include "klee/klee.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#define THRESH 0.001
#define SMALLEST_ABS 0.01

#ifdef KLEE_RUNTIME
unsigned klee_is_symbolic(uintptr_t x)
{
	return 0;
}

bool klee_is_nan_float(float f) { return isnan(f); }

bool klee_is_infinite_float(float f) { return isinf(f); }
#else
unsigned klee_is_symbolic(uintptr_t x);
#endif

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
  int n = 2;

  float inputs[n];

  klee_make_symbolic(inputs, sizeof(inputs), "inputs");

  for (int i = 0; i < n; i++) {
    klee_assume(inputs[i] > SMALLEST_ABS);

    klee_assume(fl_abs(inputs[i]) < 1024);
    klee_assume(!klee_is_nan_float(inputs[i]));
    klee_assume(!klee_is_infinite_float(inputs[i]));
  }

  float m1 = mse1(inputs, n);
  float m2 = mse2(inputs, n);

  if(!klee_is_symbolic(inputs[0])) {
    for(int i = 0; i < n; ++i) {
      printf("[%d] = %f\n", i, inputs[i]);
    }
    printf("%f : %f : %f\n", m1, m2, ((m1-m2)/m1));
  }

  klee_assert(fl_abs((m1 - m2) / m1) < THRESH);
}
