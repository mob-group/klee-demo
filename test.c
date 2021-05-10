#include "klee/klee.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define THRESH 0.001
#define SMALLEST_ABS 0.01

/*
 * KLEE defines a bunch of stuff as verification-only special functions that get
 * dealt with inside the LLVM machinery. If we want to actually *run* the
 * program with generated test cases, then we need to provide a runtime version
 * of these things.
 */

#ifdef KLEE_RUNTIME
// A bit of a trick - if you're actually executing this function, you know that
// no values are actually symbolic (i.e. they've been substituted in by the
// test case runtime).
unsigned klee_is_symbolic(uintptr_t x) { return 0; }

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

    // Uncomment the lines below to get rid of 'uninteresting' cases where your
    // assertion might break down (e.g. infinite, NaN, really large values).

    /* klee_assume(fl_abs(inputs[i]) < 1024); */
    /* klee_assume(!klee_is_nan_float(inputs[i])); */
    /* klee_assume(!klee_is_infinite_float(inputs[i])); */
  }

  float m1 = mse1(inputs, n);
  float m2 = mse2(inputs, n);

  if (!klee_is_symbolic(inputs[0])) {
    for (int i = 0; i < n; ++i) {
      printf("[%d] = %f\n", i, inputs[i]);
    }
    printf("%f : %f : %f\n", m1, m2, ((m1 - m2) / m1));
  }

  klee_assert(fl_abs((m1 - m2) / m1) < THRESH);
}
