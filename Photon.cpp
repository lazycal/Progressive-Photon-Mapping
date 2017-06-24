//
// Created by lazycal on 2017/6/20.
//

#include "Photon.h"

Photon Photon::diffused(const Vector3 &P, const Vector3 &N) const
{
    //pdf cos(x)  x \in [-pi/2, 0]
    //cdf -sin(x) x \in [-pi/2, 0]
    double x = -asin(-rand() * 1. / RAND_MAX) * (1 - rand() % 2 * 2);
    Vector3 V = N.getAnVerticalVector();
    Vector3 dir = N.rotated(V, x).rotated(N, rand() * PI * 2 / RAND_MAX);
    Photon res = *this;
    res.ray = Ray(P, dir);
    return res;
}

Photon Photon::reflected(const Vector3 &P, const Vector3 &N) const
{
    Photon res = *this;
    res.ray = res.ray.reflected(P, N);
    return res;
}

Photon Photon::refracted(const Vector3 &P, const Vector3 &N, double n, bool &isRefracted) const
{
    Photon res = *this;
    res.ray = res.ray.refracted(P, N, n, isRefracted);
    return res;
}
