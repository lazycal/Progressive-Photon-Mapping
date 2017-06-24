//
// Created by lazycal on 2017/6/15.
//

#ifndef RAY_TRACING_BEZIER_H
#define RAY_TRACING_BEZIER_H


#include <ostream>
#include "Primitive.h"
#include "Polynomial.h"

class Bezier : public Primitive{
public:
    BoundingBox bbox;
    Polynomial px, py, dpx, dpy;//p(t)
    Vector3 Dz, Dx, Dy, P;
    double S;
    std::vector<Vector3> control_points;

    Color getColor(const Vector3 &P) override;

    Collision collide(const Ray &ray) override;

    void input(const std::string &var, std::stringstream &ss) override;

    void init() override;

    Vector3 getN(double t, double theta);

    Vector3 getPoint(double t, double theta);

    std::pair<Polynomial, Polynomial> P2d(int l, int n);

    friend std::ostream &operator<<(std::ostream &os, const Bezier &bezier);

    Color getColor(double t, double theta);

    void generateMeshes(std::vector<Vector3> &points, std::vector<int4> &meshes) override;
};


#endif //RAY_TRACING_BEZIER_H
