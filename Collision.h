//
// Created by lazycal on 2017/6/11.
//

#ifndef RAY_TRACING_COLLISION_H
#define RAY_TRACING_COLLISION_H

#include <ostream>
#include "Vector3.h"

class Primitive;
struct Collision {
    Collision();
    Collision(Primitive *p);
    Color color;

    friend std::ostream &operator<<(std::ostream &os, const Collision &collision);

    bool crash;
    Vector3 P, N;
    double dist;
    Primitive *primitive;
};


#endif //RAY_TRACING_COLLISION_H
