//
// Created by lazycal on 2017/6/11.
//

#ifndef RAY_TRACING_CAMERA_H
#define RAY_TRACING_CAMERA_H

#include <ostream>
#include "Vector3.h"
#include "Ray.h"
class Camera {
public:
    Camera();

    friend std::ostream &operator<<(std::ostream &os, const Camera &camera);

    Camera(const Vector3 &O, const Vector3 &N, double lens_W, double lens_H, int W, int H);
    Ray rayCast(double i, double j);
    Vector3 O , N , Dx , Dy;
    double lens_W , lens_H;
    int W , H;

    void input(const std::string &var, std::stringstream &fin);

    Vector3 getPixelPos(Vector3 p);

    double getPixelLength(const Vector3 &P);

    void generateMeshes(std::vector<Vector3> &points, std::vector<int4> &meshes);

private:
    void init();
};


#endif //RAY_TRACING_CAMERA_H
