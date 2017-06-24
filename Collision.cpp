//
// Created by lazycal on 2017/6/11.
//

#include "Collision.h"

Collision::Collision(Primitive *p) : crash(false), primitive(p){}

Collision::Collision(): crash(false){

}

std::ostream &operator<<(std::ostream &os, const Collision &collision) {
    os << "crash: " << collision.crash << " O: " << collision.P << " N: " << collision.N << " dist: " << collision.dist
       << " primitive: " << collision.primitive;
    return os;
}
