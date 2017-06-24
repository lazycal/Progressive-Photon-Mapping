//
// Created by lazycal on 2017/6/20.
//

#ifndef RAY_TRACING_PHOTONMAP_H
#define RAY_TRACING_PHOTONMAP_H

#include <vector>
#include "Photon.h"
#include "RtHitPoint.h"
#include <algorithm>

struct Info {
    int l, r, num, id;

    Info() {}
};


class PhotonMap {
public:
    PhotonMap();

    void update(RtHitPoint &hitPoint) const;
    void query(int u, int dim, std::vector<int> &res, const Vector3 &P, double R) const;
    void addPhoton(const Photon &photon);
    void build();
    int build(int l, int r, int dim);
    void clear();
    int size();
private:
    int n;
    bool builded;

    std::vector<Photon> photons;
    std::vector<Info> nodes;
    int root;
};


#endif //RAY_TRACING_PHOTONMAP_H
