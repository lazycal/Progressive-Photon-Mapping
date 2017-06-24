//
// Created by lazycal on 2017/6/12.
//

#ifndef RAY_TRACING_LIGHT_H
#define RAY_TRACING_LIGHT_H

#include "Ray.h"
#include "Collision.h"
#include "Photon.h"

struct Light {
    int hashId;

    Light();

    Vector3 O, Dx, Dy;
    Color color;
    Photon emitPhoton() const;
    void input(std::string var, std::stringstream &fin);
    Vector3 getPos(double i, double j) const;

    void generateMeshes(std::vector<Vector3> &points, std::vector<int4> &meshes);
};


#endif //RAY_TRACING_LIGHT_H
