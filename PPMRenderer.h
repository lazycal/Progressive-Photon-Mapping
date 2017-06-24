//
// Created by lazycal on 2017/6/20.
//

#ifndef RAY_TRACING_PPMRENDERER_H
#define RAY_TRACING_PPMRENDERER_H

#include "Camera.h"
#include "header.h"
#include "World.h"
#include "RtHitPoint.h"
#include "PhotonMap.h"
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
class PPMRenderer {
public:
    int H, W, PHONG_SEC_POW;
    int RAY_TRACING_DEPTH;
    int RESAMPLE_QUALITY;
    int ITERS;
    int MAX_THREADS;
    int PHOTONS_PER_ITER;
    int PHOTON_TRACING_DEPTH;
    constexpr static double ALPHA = 0.7;
    World world;
    PPMRenderer(std::string input_file, std::string output_file);
    void run();
    void output(std::string suf="");

private:
    int rt_i, rt_j;
    std::mutex mt;
    std::string output_file;
    PhotonMap photonMap;
    Mat res, oid, rt_res;
    std::vector< std::vector<RtHitPoint> > rtHitMap;
    Color rayTrace(Ray ray, int depth, Color weight, bool inside, int &hashId, std::vector<RtHitPoint> &hitPoints, Mat res = cv::Mat());

    Color phong(const Collision &collision, const Ray &ray, int &hashId);

    Color calcRefr(const Collision &collision, const Ray &ray, int depth, const Color &weight, bool inside, int &hashId,
                   std::vector<RtHitPoint> &hitPoints, Mat &);

    void drawSegment(Vector3 u, Vector3 v, cv::Mat &img, Scalar color = cv::Scalar(0,0,255));

    bool inBoundary(int i, int j);

    int HASH_BASE, MOD;

    Color calcRefl(const Collision &collision, const Ray &ray, int depth, Color weight, bool inside, int &hashId,
                   std::vector<RtHitPoint> &hitPoints, Mat &img);

    void photonTrace(const Photon &photon, int depth, bool inside);

    bool calcPhotonRefr(const Collision &collision, const Photon &photon, int depth, bool inside, double &ksi);

    bool calcPhotonRefl(const Collision &collision, const Photon &photon, int depth, bool inside, double &ksi);

    bool calcPhotonDiff(const Collision &collision, const Photon &photon, int depth, bool inside, double &ksi);
    Scalar _color;

    void multithreadRayTrace();

    void nextRow(int &i);
};


#endif //RAY_TRACING_PPMRENDERER_H
