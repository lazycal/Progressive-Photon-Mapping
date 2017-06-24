//
// Created by lazycal on 2017/6/16.
//

#include "BoundingBox.h"

BoundingBox::BoundingBox()
{
    for (int i = 0; i < 3; ++i)
        boundary[i * 2] = INF, boundary[i * 2 + 1] = -INF;
}

std::ostream &operator<<(std::ostream &os, const BoundingBox &box)
{
    os << "boundary: ";
    for (int i = 0; i < 3; ++i)
         os << "[" << box.boundary[i * 2] << "," << box.boundary[i * 2 + 1] << "] ";
    return os;
}

void BoundingBox::include(Vector3 p)
{
    for (int i = 0; i < 3; ++i) {
        boundary[i * 2] = min(boundary[i * 2], p[i]);
        boundary[i * 2 + 1] = max(boundary[i * 2 + 1], p[i]);
    }
}

bool BoundingBox::contains(Vector3 p) const
{
    bool res = true;
    for (int i = 0; i < 3 && res; ++i)
        res &= (sign(boundary[i * 2] - p[i]) <= 0 && sign(p[i] - boundary[i * 2 + 1]) <= 0);
    return res;
}

void BoundingBox::explode()
{
    for (int i = 0; i < 3; ++i) {
        double len = boundary[i * 2 + 1] - boundary[i * 2];
        boundary[i * 2] -= len * 0.1;
        boundary[i * 2 + 1] += len * 0.1;
    }
}
