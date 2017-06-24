//
// Created by lazycal on 2017/6/15.
//

#ifndef RAY_TRACING_PICTURE_H
#define RAY_TRACING_PICTURE_H

#include "header.h"
#include "Vector3.h"
class Picture {
public:
    Vec3b *data;
    Picture(const Mat &mat);
    Picture();
    Picture& operator=(const Mat &mat);
    Vec3b* operator[](int idx)const{return data + idx * cols;}
    Color getColor(double x, double y) const;

    virtual ~Picture();

    int rows, cols;
};


#endif //RAY_TRACING_PICTURE_H
