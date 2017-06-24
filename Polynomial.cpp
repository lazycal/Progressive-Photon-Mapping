//
// Created by lazycal on 2017/6/16.
//

#include "Polynomial.h"
//#include "matlab/main.h"
double Polynomial::sum_k = 0;
int Polynomial::ks = 0, Polynomial::max_k = 0;
double Polynomial::operator()(double t)const
{
    double res = 0;
    for (int i = c.size() - 1; i >= 0; --i)
        res = res * t + c[i];
    return res;
}


Polynomial::Polynomial(double x/*= 0*/):c(1,x){}

Polynomial Polynomial::operator*(const Polynomial &o) const
{
    Polynomial res;
    res.c.resize((o.c.size() - 1) + (c.size() - 1) + 1, 0);
    for (int i = 0; i < o.c.size(); ++i)
        for (int j = 0; j < c.size(); ++j)
            res.c[i + j] += o.c[i] * c[j];
    return res;
}

Polynomial Polynomial::operator^(int x) const
{
    Polynomial res(*this);
    while (--x) res = res * *this;
    return res;
}

Polynomial Polynomial::derivative() const
{
    Polynomial res;
    res.c.resize(std::max(int(c.size() - 1), 1), 0);
    for (int i = 1; i < c.size(); ++i)
        res.c[i - 1] = c[i] * i;
    return res;
}

void Polynomial::roots(std::vector<std::complex<double>> &res, double l, double r, int depth) const//assume roots are in [0,1]
{
    //main_solve_bezier(c, res);
    //return;
    int old_size = res.size();
    Polynomial df = derivative();
    double n = c.size() * 4, intvl = (r - l) / n;
    for (int _ = -1, k; _ <= n+1; ++_) {
        double x = _ * intvl + l, b = 1;
        for (k = 1; std::abs(b) > TOL && k <= MAX_ITER; ++k) {
            double newx = x - operator()(x) / df(x);
            b = newx - x;
            x = newx;
        }
//#ifdef DEBUG
//        if (k <= MAX_ITER) {
//            ks += 1;
//            sum_k += k;
//            max_k = std::max(max_k, k);
//        }
//        if ((int(ks)) % 100 == 1) printf("roots average iter=%.3f max_iter=%d\n", sum_k/ks, max_k);
//#endif
        if (sign(operator()(x)) || x < -EPS || x > 1+EPS) continue;
        bool duplicated = false;
        for (auto i: res) if (fabs(i.real() - x) <= TOL) {duplicated = true; break;}
        if (!duplicated) res.push_back(std::complex<double>(x));

    }
//    if (n > 2 && depth < MAX_ITER_DEPTH)
//    for (int i = old_size, new_size = res.size(); i < new_size; ++i)
//        roots(res, res[i].real() - intvl / 2, res[i].real() + intvl / 2, depth + 1);
    if (n > 1 && depth < MAX_ITER_DEPTH) {
        std::vector<int> intvls(0);
        std::vector<bool> mark(static_cast<unsigned long>(n), false);
        for (int i = old_size, new_size = res.size(); i < new_size; ++i)
            if (l <= res[i].real() && res[i].real() <= r) {
                int idx = (res[i].real() - l) / intvl - 0.5;
                if (idx > 0 && !mark[idx]) {
                    mark[idx] = true;
                    intvls.push_back(idx);
                }
                idx = (res[i].real() - l) / intvl + 0.5;
                if (idx < mark.size() && !mark[idx]) {
                    mark[idx] = true;
                    intvls.push_back(idx);
                }
            }
        for (auto i : intvls)
            roots(res, i * intvl, (i + 1) * intvl, depth + 1);
    }

}
double Polynomial::root(double x) const
{return x;
    Polynomial df = derivative();
    double b = 1;
    for (int k = 1; std::abs(b) > TOL2 * std::abs(x) && k <= MAX_ITER2; ++k) {
        double newx = x - operator()(x) / df(x);
        b = newx - x;
        x = newx;
    }
    return x;
}
Polynomial Polynomial::operator/(double x) const
{
    Polynomial res(*this);
    for (auto &i:res.c) i /= x;
    return res;
}

Polynomial Polynomial::operator-(const Polynomial &o) const
{
    Polynomial res(*this);
    if (o.c.size() > res.c.size()) res.c.resize(o.c.size(), 0);
    for (int i = 0; i < o.c.size(); ++i)
        res.c[i] -= o.c[i];
    return res;
}
Polynomial Polynomial::operator+(const Polynomial &o) const
{
    Polynomial res(*this);
    if (o.c.size() > res.c.size()) res.c.resize(o.c.size(), 0);
    for (int i = 0; i < o.c.size(); ++i)
        res.c[i] += o.c[i];
    return res;
}

Polynomial Polynomial::operator+(double x) const
{
    return Polynomial(x) + *this;
}

Polynomial operator+(double x, const Polynomial &o)
{
    return o + x;
}

Polynomial pow(const Polynomial &o, int x)
{
    return (o^x);
}

Polynomial operator*(double x, const Polynomial &o)
{
    return o*x;
}

Polynomial::Polynomial(double x, double y): c(2)
{
    c[0] = x; c[1] = y;
}
