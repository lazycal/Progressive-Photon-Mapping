//
// Created by lazycal on 2017/6/11.
//

#ifndef RAY_TRACING_WORLD_H
#define RAY_TRACING_WORLD_H

#include "Ray.h"
#include <vector>
#include <string>
#include "Camera.h"
#include "Vector3.h"
#include "Primitive.h"
#include "Collision.h"
#include "Sphere.h"
#include "Light.h"

class World {
public:
    World(std::string input_file);
    ~World();
    Light *light;
    Camera camera;
    Collision findNearestPrimitive(const Ray &ray);
    Color bgc;

    int SHADE_QUALITY;
    Color getRadiance(const Vector3 &P, const Vector3 &N);

    double getRadianceWithLight(const Vector3 &P, const Vector3 &N, const Vector3 &O);
    std::vector<Vector3> points;
    std::vector<int4> meshes;
private:
    std::vector<Primitive*> pris;


};



#endif //RAY_TRACING_WORLD_H
