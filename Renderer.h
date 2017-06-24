//
// Created by lazycal on 2017/6/11.
//

#ifndef RAY_TRACING_RENDER_H
#define RAY_TRACING_RENDER_H

#include "Camera.h"
#include "header.h"
#include "World.h"
#include <string>
#include <iostream>
class Renderer {
public:
    const static int MAX_DEPTH = 20;
    int H, W, PHONG_SEC_POW;
    int RAY_TRACING_DEPTH;
    int RESAMPLE_QUALITY;
    double PHONG_ENVIR; //环境光系数
    World world;
    Renderer(std::string input_file, std::string output_file);
    void run();
    void output(std::string suf="");

private:
    Mat res, oid;
    std::string output_file;

    Color rayTrace(Ray ray, int depth, Color weight, bool inside, int &hashId, Mat res = cv::Mat());

    Color phong(const Collision &collision, const Ray &ray, int &hashId);

    Color calcRefl(const Collision &collision, const Ray &ray, int depth, Color weight, bool inside, int &hashId, Mat &);

    Color calcRefr(const Collision &collision, const Ray &ray, int depth, const Color &weight, bool inside, int &hashId, Mat &);

    void drawSegment(Vector3 u, Vector3 v, cv::Mat &img, Scalar color = cv::Scalar(0,0,255));

    bool inBoundary(int i, int j);

    int HASH_BASE;
};


#endif //RAY_TRACING_RENDER_H
