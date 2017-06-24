//
// Created by lazycal on 2017/6/11.
//

#include "Sphere.h"

Collision Sphere::collide(const Ray &ray)
{
    Collision collision(this);
    if (sign(ray.dis2(this->center) - this->r * this->r) >= 0)
        return collision;
    Vector3 l = center - ray.O;
    double d2 = l.cross(ray.V).len2() / ray.V.len2();
    double tp = l.dot(ray.V); // > 0 <90 ; < 0 > 90
    double x = sqrt(r * r - d2);
    if (sign(l.len2() - r * r) <= 0) {
        collision.P = ray.travel(tp + x);
        collision.dist = tp + x;
    }else {
        collision.P = ray.travel(tp - x);
        collision.dist = tp - x;
    }
    collision.N = (collision.P - center).normalized();
    collision.N *= -sign(collision.N.dot(ray.V));
    collision.crash = true;
    collision.color = getColor(collision.P);
    assert(collision.dist > 0);
    return collision;
}

Color Sphere::getColor(const Vector3 &_P)
{
    if (texture.data == NULL) return color;
    Vector3 P = (_P - center) / r;
    double x, y;
    if (!sign(P.x) && !sign(P.y)) x = 0, y = 0;
    else x = atan2(P.y, P.x)/2/PI + 0.5, y = asin(P.z) / PI + 0.5;
#ifdef DEBUG
    assert(sign(x) >= 0 && sign(y) >=0 && sign(x - 1) <= 0 && sign(y - 1) <= 0);
#endif
    return texture.getColor(x, y);
}

void Sphere::input(const std::string &var, std::stringstream &fin) {
    if ( var == "O=" ) fin >> center;
    else if ( var == "R=" ) fin >> r;
    else Primitive::input(var, fin);
}

void Sphere::init()
{
    for (int i = -1; i < 2; i += 2)
        for (int j = -1; j < 2; j += 2)
            for (int k = -1; k < 2; k += 2)
                bbox.include(center + Vector3(i * r, j * r, k * r));
    Primitive::init();
}

void Sphere::generateMeshes(std::vector<Vector3> &points, std::vector<int4> &meshes)
{
    float du = 0.01f, dv = 0.01f;             // 自定义密度
    int nu = 1 / du + 1, nv = 1 / dv + 1, offset = points.size();   // 密度应该“除”得整
    int pid = offset;                            // 点序号
    for (float u = 0.0f, i = 0; u <= 1.0f; u += du, i++) {
        for (float v = 0.0f, j = 0; v <= 1.0f; v += dv, j++) {
            points.push_back(getPoint(u, v));          // 写递归或者DP搞定函数P的计算
            pid++;                              // OBJ格式网格序号从1开始
            if (i != 0 && j != 0) {
                //printf("f %d %d %d %d\n", pid-nv-1, pid-nv, pid, pid-1);
                meshes.push_back(int4(pid-nv-1, pid-1, pid, pid-nv));
            }
        }
    }

}

Vector3 Sphere::getPoint(double u, double v)
{
    double theta = u * 2 * PI, phi = (v - .5) * PI;
    return Vector3(cos(theta) * cos(phi), sin(theta) * cos(phi), sin(phi)) * r + center;
}
