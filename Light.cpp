//
// Created by lazycal on 2017/6/12.
//

#include "Light.h"

void Light::input(std::string var, std::stringstream &fin) {
    if (var == "color=") fin >> color;
    if (var == "O=") fin >> O;
    if (var == "Dx=") fin >> Dx;
    if (var == "Dy=") fin >> Dy;
}

Light::Light()
{
    hashId = rand();
}

Vector3 Light::getPos(double i, double j) const
{
    return j * Dx - i * Dy + O;
}

Photon Light::emitPhoton() const
{
    double x = 1 - rand() * 2. / RAND_MAX;
    double y = 1 - rand() * 2. / RAND_MAX;
    Vector3 P = getPos(x, y);
    Photon photon;
    photon.ray.O = P;
    photon.flux = color;// / color.power();
    //return photon.diffused(P, Dx.cross(Dy).normalized()* (rand() % 2 ? 1 : -1));
    photon.ray.V = Vector3::randomVector();
    return photon;
}


void
Light::generateMeshes(std::vector<Vector3> &points, std::vector<int4> &meshes)
{
    int pid = points.size() + 1;
    for (int i = -1; i <= 1; i += 2)
        for (int j = -1; j <= 1; j += 2)
            points.push_back(O + (Dx * i + Dy * j));//(-1, -1) (-1, 1) (1, -1) (1, 1)
    meshes.push_back(int4(pid, pid + 2, pid + 3, pid + 1));
}