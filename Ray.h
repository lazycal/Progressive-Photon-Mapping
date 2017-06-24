//
// Created by lazycal on 2017/6/11.
//

#ifndef RAY_TRACING_RAY_H
#define RAY_TRACING_RAY_H

#include <ostream>
#include "Vector3.h"
#include "BoundingBox.h"
struct Ray {
    friend std::ostream &operator<<(std::ostream &os, const Ray &ray);

    Ray(const Vector3 &O, const Vector3 &V);

    Ray();

    Vector3 O, V;
    Vector3 travel(double dis) const;
    double dis2(const Vector3 &p) const;
    Ray reflected(const Vector3 &P, const Vector3 &N) const;
    Ray refracted(const Vector3 &P, const Vector3 &N, double n, bool &isRefracted) const;
    bool crash(const BoundingBox &bbox) const;
};


#endif //RAY_TRACING_RAY_H
