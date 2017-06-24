//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xscal.h
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 16-Jun-2017 11:39:57
//
#ifndef XSCAL_H
#define XSCAL_H

// Include Files
#include <cmath>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "solve_bezier_types.h"

// Function Declarations
extern void b_xscal(int n, const creal_T a, emxArray_creal_T *x, int ix0, int
                    incx);
extern void xscal(int n, const creal_T a, emxArray_creal_T *x, int ix0);

#endif

//
// File trailer for xscal.h
//
// [EOF]
//
