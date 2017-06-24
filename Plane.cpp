//
// Created by lazycal on 2017/6/12.
//

#include "Plane.h"
// ray.O + t * ray.V = O(t)
// N.*O(t) + D = 0
// N.*ray.O + N.* t*ray.V + D = 0
// t = -(N.*ray.O + D) / (N.*ray.V)
Collision Plane::collide(const Ray &ray) {
    double x = N.dot(ray.V);
    Collision coll(this);
    if (!sign(x)) return coll;
    double t = -(D + N.dot(ray.O)) / x;
    if (sign(t) <= 0) return coll;
    coll.crash = true;
    coll.dist = t;
    coll.N = N * -sign(N.dot(ray.V));
    coll.P = ray.travel(t);
    coll.color = getColor(coll.P);
    return coll;
}

void Plane::generateMeshes(std::vector<Vector3> &points, std::vector<int4> &meshes)
{
    Vector3 Dx, Dy;
    Dx = Vector3(0, 1, 0).cross(N);
    if (Dx == 0) Dx = Vector3(1, 0, 0);
    else Dx = Dx.normalized();
    Dy = N.cross(Dx);
    int pid = points.size() + 1;
    for (int i = -1; i <= 1; i += 2)
        for (int j = -1; j <= 1; j += 2)
            points.push_back(O + 1e1 * (Dx * i + Dy * j));//(-1, -1) (-1, 1) (1, -1) (1, 1)
    meshes.push_back(int4(pid, pid + 2, pid + 3, pid + 1));
}

void Plane::input(const std::string &var, std::stringstream &ss) {
    if (var == "Dx=") ss >> Dx;
    else if (var == "Dy=") ss >> Dy;
    else if (var == "N=") ss >> N;
    else if (var == "R=") ss >> r;
    else Primitive::input(var, ss);
    init();
}

void Plane::init() {
    N = N.normalized();
    O = r * N;
    D = -N.dot(O); //N.*O + D = 0
}

Color Plane::getColor(const Vector3 &P)
{
    if (texture.data == NULL) return color;
    double x = std::abs((P - O).dot(Dx)) / Dx.len2(), y = std::abs((P - O).dot(Dy)) / Dy.len2();
    x -= floor(x); y -= floor(y);
#ifdef DEBUG
    assert(sign(x) >= 0 && sign(y) >=0 && sign(x - 1) <= 0 && sign(y - 1) <= 0);
#endif
    return texture.getColor(x, y);
}
