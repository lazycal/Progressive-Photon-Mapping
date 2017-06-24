//
// Created by lazycal on 2017/6/16.
//

#ifndef RAY_TRACING_BOUNDINGBOX_H
#define RAY_TRACING_BOUNDINGBOX_H

#include <ostream>
#include "Vector3.h"
class BoundingBox {
public:
    double boundary[6]; // lx rx ly ry lz rz
    BoundingBox();
    void include(Vector3 p);
    void explode();
    bool contains(Vector3 p) const;

    friend std::ostream &operator<<(std::ostream &os, const BoundingBox &box);
};


#endif //RAY_TRACING_BOUNDINGBOX_H
