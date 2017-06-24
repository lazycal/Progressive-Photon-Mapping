//
// Created by lazycal on 2017/6/16.
//

#ifndef RAY_TRACING_POLYNOMIAL_H
#define RAY_TRACING_POLYNOMIAL_H

#include <vector>
#include "Vector3.h"
#include <complex>
#include "matlab/main.h"
class Polynomial {
    static double sum_k;
    static int ks, max_k;
public:
    constexpr const static double TOL = 1e-9;
    const static int MAX_ITER = 30;
    constexpr const static double TOL2 = 1e-9;
    const static int MAX_ITER2 = 30;
    const static int MAX_ITER_DEPTH = 5;
    std::vector<double>c;
    double operator()(double t)const;
    Polynomial operator*(const Polynomial &o)const;
    Polynomial operator/(double x)const;
    Polynomial operator-(const Polynomial &o)const;
    Polynomial operator+(const Polynomial &o)const;
    Polynomial operator+(double x)const;
    friend Polynomial operator+(double x, const Polynomial &o);
    friend Polynomial operator*(double x, const Polynomial &o);
    friend Polynomial pow(const Polynomial &o, int x);
    Polynomial operator^(int x)const;
    Polynomial derivative()const;
    //std::vector<std::complex<double>> roots(double l = 0, double r = 1) const;

    Polynomial(double x, double y);
    Polynomial(double x = 0);

    double root(double x) const;

    void roots(std::vector<std::complex<double> > &res, double l = 0, double r = 1, int depth = 1) const;

};


#endif //RAY_TRACING_POLYNOMIAL_H
