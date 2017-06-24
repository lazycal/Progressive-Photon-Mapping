//
// Created by lazycal on 2017/6/20.
//

#ifndef RAY_TRACING_PHOTON_H
#define RAY_TRACING_PHOTON_H


#include "Ray.h"

class Photon {
public:
    Color flux;
    Ray ray;
    Photon diffused(const Vector3 &P, const Vector3 &N) const;
    Photon reflected(const Vector3 &P, const Vector3 &N) const;
    Photon refracted(const Vector3 &P, const Vector3 &N, double n, bool &isRefracted) const;
};


#endif //RAY_TRACING_PHOTON_H
