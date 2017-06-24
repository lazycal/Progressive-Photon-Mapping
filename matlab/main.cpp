//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: main.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 16-Jun-2017 11:39:57
//

//***********************************************************************
// This automatically generated example C main file shows how to call
// entry-point functions that MATLAB Coder generated. You must customize
// this file for your application. Do not modify this file directly.
// Instead, make a copy of this file, modify it, and integrate it into
// your development environment.
//
// This file initializes entry-point function arguments to a default
// size and value before calling the entry-point functions. It does
// not store or use any values returned from the entry-point functions.
// If necessary, it does pre-allocate memory for returned values.
// You can use this file as a starting point for a main function that
// you can deploy in your application.
//
// After you copy the file, and before you deploy it, you must make the
// following changes:
// * For variable-size function arguments, change the example sizes to
// the sizes that your application requires.
// * Change the example values of function arguments to the values that
// your application requires.
// * If the entry-point functions return values, store these values or
// otherwise use them as required by your application.
//
//***********************************************************************
// Include Files
#include "rt_nonfinite.h"
#include "solve_bezier.h"
#include "main.h"
#include "solve_bezier_terminate.h"
#include "solve_bezier_emxAPI.h"
#include "solve_bezier_initialize.h"
#include <cstdio>

// Function Declarations
static emxArray_real_T *argInit_1xUnbounded_real_T();
static double argInit_real_T();
static void main_solve_bezier();

// Function Definitions

//
// Arguments    : void
// Return Type  : emxArray_real_T *
//
static emxArray_real_T *argInit_1xUnbounded_real_T(int n, const std::vector<double> &a)
{
  emxArray_real_T *result;
  static int iv0[2] = { 1, n };

  int idx1;

  // Set the size of the array.
  // Change this size to the value that the application requires.
  result = emxCreateND_real_T(2, *(int (*)[2])&iv0[0]);

  // Loop over the array to initialize each element.
  for (idx1 = 0; idx1 < result->size[1U]; idx1++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result->data[result->size[0] * idx1] = a[idx1];
  }

  return result;
}

//
// Arguments    : void
// Return Type  : double
//
static double argInit_real_T()
{
  return 0.0;
}

//
// Arguments    : void
// Return Type  : void
//
void main_solve_bezier(const std::vector<double> &a, std::vector<std::complex<double> > &b)
{
  emxArray_creal_T *t;
  emxArray_real_T *C;
  emxInitArray_creal_T(&t, 1);

  // Initialize function 'solve_bezier' input arguments.
  // Initialize function input argument 'C'.
  int n = a.size();
  C = argInit_1xUnbounded_real_T(n, a);

  // Call the entry-point 'solve_bezier'.
  solve_bezier(C, t);
  b.clear();
  for (int i = 0; i < t->size[0]; ++i)
    b.push_back(std::complex<double>(t->data[i].re, t->data[i].im));
  emxDestroyArray_creal_T(t);
  emxDestroyArray_real_T(C);
}
