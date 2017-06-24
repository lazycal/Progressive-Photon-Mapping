//
// Created by lazycal on 2017/6/12.
//

#ifndef RAY_TRACING_PLAIN_H
#define RAY_TRACING_PLAIN_H


#include "Primitive.h"

class Plane : public Primitive{
public:
    Vector3 N;
    double r;
    Vector3 Dx, Dy;
    Collision collide(const Ray &ray) override;

    void input(const std::string &var, std::stringstream &ss) override;

//protected:
    double D;
    Vector3 O;
    void init() override;

    Color getColor(const Vector3 &P) override;
    void generateMeshes(std::vector<Vector3> &points, std::vector<int4> &meshes) override;
};


#endif //RAY_TRACING_PLAIN_H
