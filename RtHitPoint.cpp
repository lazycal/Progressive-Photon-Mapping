//
// Created by lazycal on 2017/6/20.
//

#include "RtHitPoint.h"

RtHitPoint::RtHitPoint(const Vector3 &O, const Vector3 &N, const Vector3 &weight, double R, Primitive *primitive, Color color) :
        O(O), N(N), wgt(weight), R(R), primitive(primitive), color(color), flux(0)
{
    photons = 0;
}

RtHitPoint::RtHitPoint() {}
