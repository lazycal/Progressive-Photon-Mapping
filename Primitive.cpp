//
// Created by lazycal on 2017/6/11.
//

#include "Primitive.h"
int Primitive::cnt = 0;
void Primitive::input(const std::string &var, std::stringstream &fin) {
    if ( var == "color=" ) fin >> color;
    if ( var == "absor=" ) fin >> absor;
    if ( var == "refl=" ) fin >> refl;
    if ( var == "refr=" ) fin >> refr;
    if ( var == "diff=" ) fin >> diff;
    if ( var == "spec=" ) fin >> spec;
    if ( var == "rindex=" ) fin >> rindex;
    if ( var == "texture=" ) {
        std::string file; fin >> file;
        texture = imread(file);
        assert(texture.data != NULL);
    }
}

Primitive::Primitive() {
    hashId = rand();
    color = absor = Color();
    refl = refr = diff = spec = 0;
    rindex = 0;
    id = cnt++;
}

Primitive::~Primitive() {

}

std::ostream &operator<<(std::ostream &os, const Primitive &primitive) {
    os << "color: " << primitive.color << " absor: " << primitive.absor << " refl: " << primitive.refl << " refr: "
       << primitive.refr << " diff: " << primitive.diff << " spec: " << primitive.spec << " rindex: "
       << primitive.rindex << " id: " << primitive.id;
    return os;
}

Color Primitive::getColor(const Vector3 &P) {
    return color;
}

void Primitive::generateMeshes(std::vector<Vector3> &points, std::vector<int4> &meshes)
{


}
