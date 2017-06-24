//
// Created by lazycal on 2017/6/11.
//

#include "Camera.h"
#include "Sphere.h"

Camera::Camera()
{
    H = 0;
    W = 0;
    O = Vector3(0, 0, -1);
    N = Vector3(0, 0, 1);
    init();
}

Camera::Camera(const Vector3 &O, const Vector3 &N, double lens_W, double lens_H, int W, int H)
        : O(O), N(N), lens_W(lens_W), lens_H(lens_H), W(W), H(H)
{
    init();
}

void Camera::init()
{
    N = N.normalized();
    Dx = N.cross(Vector3(0, 0, 1));
    if (Dx == Vector3(0)) Dx = Vector3(1, 0, 0);
    else Dx = Dx.normalized();
    Dy = -N.cross(Dx);
}

Ray Camera::rayCast(double i, double j)
{
    return Ray(O, N - Dy * lens_H * (i * 1.0 / H - 0.5) + Dx * lens_W * (j * 1.0 / W - 0.5));
}

void Camera::input(const std::string &var, std::stringstream &fin) {
    if ( var == "O=" ) fin >> O;
    if ( var == "N=" ) fin >> N;
    if ( var == "Dx=" ) fin >> Dx;
    if ( var == "lens_W=" ) fin >> lens_W;
    if ( var == "lens_H=" ) fin >> lens_H;
    if ( var == "image_W=" ) fin >> W;
    if ( var == "image_H=" ) fin >> H;
    init();
}

std::ostream &operator<<(std::ostream &os, const Camera &camera) {
    os << "O: " << camera.O << " N: " << camera.N << " Dx: " << camera.Dx << " Dy: " << camera.Dy << " lens_W: "
       << camera.lens_W << " lens_H: " << camera.lens_H << " W: " << camera.W << " H: " << camera.H;
    return os;
}

Vector3 Camera::getPixelPos(Vector3 p) {
    p -= O;
    p = Vector3(Dx.dot(p), Dy.dot(p), N.dot(p));
    p /= p[2];
    return Vector3((p[0] / lens_W + 0.5) * W, (-p[1] / lens_H + 0.5) * H, 1);
}

double Camera::getPixelLength(const Vector3 &P)
{
    return 0.2;
    double dis = (P - O).len();
    return dis * sqrt(sqr(lens_H*0.01) + sqr(lens_W*0.01));
}

void
Camera::generateMeshes(std::vector<Vector3> &points, std::vector<int4> &meshes)
{
    Sphere sphere;
    sphere.center = O;
    sphere.r = 0.05;
    sphere.generateMeshes(points, meshes);
    int pid = points.size() + 1;
    for (int i = -1; i <= 1; i += 2)
        for (int j = -1; j <= 1; j += 2)
            points.push_back(O + (N + (Dx * i * lens_W + Dy * lens_H * j)) * 0.3);//(-1, -1) (-1, 1) (1, -1) (1, 1)
    meshes.push_back(int4(pid, pid + 2, pid + 3, pid + 1));

}
