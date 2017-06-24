//
// Created by lazycal on 2017/6/20.
//

#include "PhotonMap.h"
#include "PPMRenderer.h"

void PhotonMap::addPhoton(const Photon &photon)
{
    photons.push_back(photon);
}

PhotonMap::PhotonMap() : nodes(0), photons(1)
{
    n = 0;
}

void PhotonMap::clear()
{
    photons.resize(1);
}

void PhotonMap::build()
{
    n = photons.size() - 1;
    nodes.resize(n);
    root = build(1, n, 0);
}

int PhotonMap::build(int l, int r, int dim)
{
    if (l > r) return 0;
    std::sort(photons.begin() + l, photons.begin() + r + 1,
              [dim](const Photon &lhs, const Photon &rhs) { return lhs.ray.O[dim] < rhs.ray.O[dim]; });
    int mid = (l + r) / 2;
    nodes[mid].l = build(l, mid - 1, (dim + 1) % 3);
    nodes[mid].r = build(mid + 1, r, (dim + 1) % 3);
    return mid;
}

void PhotonMap::query(int u, int dim, std::vector<int> &res, const Vector3 &P, double R) const
{
    if (!u) return;
    Vector3 Pu = photons[u].ray.O;
    double dis2 = (Pu - P).len2();
    if (dis2 <= R * R) res.push_back(u);
    if (Pu[dim] - P[dim] <= R) query(nodes[u].r, (dim + 1) % 3, res, P, R);
    if (P[dim] - Pu[dim] <= R) query(nodes[u].l, (dim + 1) % 3, res, P, R);
}

void PhotonMap::update(RtHitPoint &hitPoint) const
{
    std::vector<int> res;
    query(root, 0, res, hitPoint.O, hitPoint.R);
    //cout << res.size() << '\n';
//    int _ = 0;
//    for (int i = 1; i <= n; ++i)
//        if ((photons[i].ray.O - hitPoint.O).len2() <= hitPoint.R * hitPoint.R)
//            ++_;
//    assert(_ == res.size());
    Color fluxm;
    int m = 0;
    for (auto photon : res) {
#ifdef DEBUG
        assert((photons[photon].ray.O - hitPoint.O).len2() <= hitPoint.R * hitPoint.R);
#endif
        if (photons[photon].ray.V.dot(hitPoint.N) < 0)
            fluxm += photons[photon].flux, ++m;
    }
    if (hitPoint.photons == 0) {
        hitPoint.flux = fluxm;
        hitPoint.photons = m;
    }else {
        double tmp = (hitPoint.photons + PPMRenderer::ALPHA * m) / (hitPoint.photons + m);
        hitPoint.flux = (hitPoint.flux + fluxm) * tmp;
        hitPoint.R *= sqrt(tmp);
        hitPoint.photons += PPMRenderer::ALPHA * m;
    }
}

int PhotonMap::size()
{
    return photons.size() - 1;
}


