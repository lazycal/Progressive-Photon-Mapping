//
// Created by lazycal on 2017/6/11.
//

#include "World.h"
#include <sstream>
#include <fstream>
#include "Plane.h"
#include "Bezier.h"

World::World(std::string input_file) {
    SHADE_QUALITY = 4;
    if ( input_file == "" ) return;
    std::ifstream fin( input_file.c_str() );

    std::string obj;
    while ( fin >> obj ) {
        Primitive* new_primitive = NULL;
        Light* new_light = NULL;

        if ( obj == "primitive" ) {
            std::string type;
            fin >> type;
            if (type == "sphere") new_primitive = new Sphere;
            if (type == "plane" ) new_primitive = new Plane;
            if (type == "bezier") new_primitive = new Bezier;
            if (new_primitive != NULL) pris.push_back(new_primitive);
        }
//            if ( type == "rectangle" ) new_primitive = new Rectangle;
//            if ( type == "triangle" ) new_primitive = new Triangle;
//            if ( type == "polyhedron" ) new_primitive = new Polyhedron;
//            AddPrimitive( new_primitive );
//        }
        else
        if ( obj == "light" ) {
            std::string type; fin >> type;
            new_light = new Light;
//            if ( type == "point" ) new_light = new PointLight;
//            if ( type == "area" ) new_light = new AreaLight;
//            AddLight( new_light );
            light = new_light;
        } else
        if (obj == "end") break;
        else if ( obj != "background" && obj != "camera" ) continue;
        std::cout << obj << std::endl;

        fin.ignore( 1024 , '\n' );

        std::string order;
        while ( std::getline( fin , order , '\n' ) ) {
            std::stringstream fin2( order );
            std::string var; fin2 >> var;
            if ( var == "end" ) {
                //if (obj == "primitive" && new_primitive != NULL) new_primitive->PreTreatment();
                break;
            }

            if ( obj == "background") fin2 >> bgc;
            if ( obj == "primitive" && new_primitive != NULL ) new_primitive->input( var , fin2 );
            if ( obj == "light" && new_light != NULL ) new_light->input( var , fin2 );
            if ( obj == "camera" ) camera.input( var , fin2 );
        }
    }
    for (auto i : pris) {
        i->init();
        i->generateMeshes(points, meshes);
        std::cout << *i << '\n';
    }
    camera.generateMeshes(points, meshes);
    light->generateMeshes(points, meshes);
}

World::~World() {
    for (auto i : pris)
        delete i;
    delete light;
}

Collision World::findNearestPrimitive(const Ray &ray) {
    Collision res(nullptr);
    res.crash = false;
    res.dist = INF;
    for (auto i : pris) {
        Collision collision(i->collide(ray));
        if (collision.crash && collision.dist < res.dist)
            res = collision;
    }
    return res;
}
double World::getRadianceWithLight(const Vector3 &P, const Vector3 &N, const Vector3 &O)
{
    Vector3 L = (O - P);
    if (sign(L.dot(N)) < EPS) return 0;
    Collision collision = findNearestPrimitive(Ray(P, L));
    if (collision.crash && sign(collision.dist - L.len()) < 0) return 0;
    else return 1;
}
Color World::getRadiance(const Vector3 &P, const Vector3 &N)
{
    double res = 0;
    for (int i = -SHADE_QUALITY; i <= SHADE_QUALITY; ++i) {
        for (int j = -SHADE_QUALITY; j <= SHADE_QUALITY; ++j) {
            Vector3 O = light->getPos(i*1.0/SHADE_QUALITY, j*1.0/SHADE_QUALITY);
            res += getRadianceWithLight(P, N, O);
        }
    }
    return res / (2 * SHADE_QUALITY + 1) / (2 * SHADE_QUALITY + 1) * light->color / light->color.power();
}
