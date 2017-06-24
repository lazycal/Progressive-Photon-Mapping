//
// Created by lazycal on 2017/6/11.
//

#ifndef RAY_TRACING_PRIMITIVE_H
#define RAY_TRACING_PRIMITIVE_H

//#include "World.h"
#include <ostream>
#include "Collision.h"
#include "Ray.h"
#include "Picture.h"
#include "header.h"
class Primitive {
public:
    static int cnt;
    friend std::ostream &operator<<(std::ostream &os, const Primitive &primitive);

    virtual Color getColor(const Vector3 &P);

private:
    friend class World;
public:
    virtual void init(){}
    Color color, absor; //物品色彩、透明物品吸收的色光
    int hashId, id;
    double refl, refr; //反射、折射
    double diff, spec; //漫反射、镜面漫反射的百分比
    double rindex; //折射率
    Picture texture;
    virtual Collision collide(const Ray &ray) = 0;
    virtual void input(const std::string &var, std::stringstream &ss);
    //double BRDF(const Vector3 &N, const Vector3 &L, const Vector3 &V) const;
    virtual void generateMeshes(std::vector<Vector3> &points, std::vector<int4> &meshes);
protected:
    Primitive();
    virtual ~Primitive();


};



#endif //RAY_TRACING_PRIMITIVE_H
