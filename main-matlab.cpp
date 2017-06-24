#include <iostream>
#include "Renderer.h"
#include "Plane.h"
#include "Polynomial.h"

using namespace std;
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
#include "matlab/rt_nonfinite.h"
#include "matlab/solve_bezier.h"
#include "matlab/main.h"
#include "matlab/solve_bezier_terminate.h"
#include "matlab/solve_bezier_emxAPI.h"
#include "matlab/solve_bezier_initialize.h"
#include "PPMRenderer.h"
#include <cstdio>

int main2()
{
//    Plane plane;
//    plane.N = Vector3(0,0,1);
//    plane.r = -2;
//    plane.init();
//    Ray ray(Vector3(0,0,-3), Vector3(1,1,0));
//    cout << plane.collide(ray);
//    Vector3 a(sqrt(3),1,0);
//    a = -a.normalized();
//    bool tmp = false;
//    cout << a.refracted(Vector3(1,0,0),0.5,tmp) << ' ' << tmp << endl;
//    Polynomial f(3, -5);
//    f.c.push_back(1.5);
//    for (auto i : f.roots(0, 0))
//        printf("%.10f,%.10f\n", i.real(), f(i.real()));

}
int main(int, const char * const [])
{
    // Initialize the application.
    // You do not need to do this more than one time.
    solve_bezier_initialize();

    double st = clock();
    PPMRenderer renderer("../../scene2.txt", "res");
    //std::cout<<renderer.world.camera<<std::endl;
    //Renderer renderer("input.txt", "res");
    renderer.run();
    cout << (clock() - st) / CLOCKS_PER_SEC << "s\n";
    solve_bezier_terminate();
    return 0;

}

//
// File trailer for main.cpp
//
// [EOF]
//
