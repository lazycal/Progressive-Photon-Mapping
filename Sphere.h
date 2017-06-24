//
// Created by lazycal on 2017/6/11.
//

#ifndef RAY_TRACING_SPHERE_H
#define RAY_TRACING_SPHERE_H

#include "Primitive.h"
#include <string>
#include <sstream>
class Sphere: public Primitive {
public:
    Vector3 center;
    double r;
    Collision collide(const Ray &ray) override;
    void input(const std::string &var, std::stringstream& ss) override;

    Color getColor(const Vector3 &P) override;

    void init() override;

    void generateMeshes(std::vector<Vector3> &points, std::vector<int4> &meshes) override;

public:

    BoundingBox bbox;

    Vector3 getPoint(double u, double v);
};


#endif //RAY_TRACING_SPHERE_H
