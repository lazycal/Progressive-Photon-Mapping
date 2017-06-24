//
// Created by lazycal on 2017/6/11.
//

#include "Ray.h"

double Ray::dis2(const Vector3 &p) const {
    if ((p - O).dot(V) > 0) return (p - O).cross(V).len2() / V.len2();//锐角
    else return (p - O).len2();
}

Ray::Ray(const Vector3 &O, const Vector3 &V) : O(O), V(V)
{
    this->V = V.normalized();
}

std::ostream &operator<<(std::ostream &os, const Ray &ray) {
    os << "O: " << ray.O << " V: " << ray.V;
    return os;
}

Vector3 Ray::travel(double dis) const {
    return O + V * dis;
}

Ray Ray::reflected(const Vector3 &P, const Vector3 &N) const {
#ifdef DEBUG
    assert(!sign(V.len() - 1));
#endif
    return Ray(P, V.reflected(N));
}

Ray Ray::refracted(const Vector3 &P, const Vector3 &N, double n, bool &isRefracted) const {
    return Ray(P, V.refracted(N, n, isRefracted));
}

bool Ray::crash(const BoundingBox &bbox) const
{
//    static int tot = 0, res_false = 0;
//    ++tot;
    if (bbox.contains(O)) return true;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 2; ++j) {
            double val = bbox.boundary[i * 2 + j], t;
            if (!sign(V[i])) continue;
            t = (val - O[i]) / V[i];
            if (sign(t) < 0) continue;
            if (bbox.contains(travel(t))) return true;
        }
    //res_false++;
    //printf("%d %d %.3f\n", res_false, tot, res_false * 1.0 / tot);
    return false;
}

Ray::Ray() {}
