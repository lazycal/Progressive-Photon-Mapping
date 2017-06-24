//
// Created by lazycal on 2017/6/20.
//

#ifndef RAY_TRACING_RTHITPOINT_H
#define RAY_TRACING_RTHITPOINT_H


#include "Vector3.h"
#include "Primitive.h"

class RtHitPoint {
public:
    RtHitPoint(const Vector3 &O, const Vector3 &N, const Vector3 &weight, double R, Primitive *primitive, Color color);
    Vector3 O, N;
    Vector3 wgt;
    double R;
    Primitive *primitive;
    Color color;
    int photons;
    Color flux;
    RtHitPoint();
};


#endif //RAY_TRACING_RTHITPOINT_H
