//
// Created by lazycal on 2017/6/11.
//

#include <fstream>
#include "Renderer.h"
//#define SHOW_TRACE

using cv::Mat;
using namespace std;

Scalar _color;

Renderer::Renderer(std::string input_file, std::string output_file) : world(input_file), output_file(output_file)
{
    HASH_BASE = 92797;
    RESAMPLE_QUALITY = 1;
    H = world.camera.H;
    W = world.camera.W;
    oid = Mat(H, W, CV_32S, Scalar::all(0));
    PHONG_ENVIR = 0.2;
    PHONG_SEC_POW = 50;
    RAY_TRACING_DEPTH = 30;
    res.create(H, W, CV_8UC3);
    FILE* fout = fopen((output_file + ".obj").c_str(), "w");
    for (auto i: world.points) fprintf(fout, "v %.10f %.10f %.10f\n", i.x, i.y, i.z);
    for (auto i: world.meshes) fprintf(fout, "f %d %d %d %d\n", i.data[0], i.data[1], i.data[2], i.data[3]);
    fclose(fout);
}

void Renderer::output(string suf)
{
    assert(res.rows > 0 && res.cols > 0);
    imwrite(output_file+suf+".bmp", res);
}

void Renderer::run()
{//518 472
    res = imread("res.bmp");
    for (int i = 0; i < H; ++i) {
        printf("rendering row %d\n", i);
        for (int j = 0 ; j < W; ++j) {
#ifdef SHOW_TRACE
            printf("\nrendering row %d col %d\n", i, j);
            _color = Scalar(255, 0, 0);
#endif
            Color vec = rayTrace(world.camera.rayCast(i, j), RAY_TRACING_DEPTH, 1, false, oid.at<int>(i, j), res) * 255;
            res.at<cv::Vec3b>(i, j) = cv::Vec3b(std::min(255.0, vec[2]), std::min(255.0, vec[1]),
                                                std::min(255.0, vec[0]));
#ifdef SHOW_TRACE
            cout << res.at<cv::Vec3b>(i, j) << endl;
#endif
        }
        if (i % 100 == 0) imwrite("tmp.bmp", res);
    }
    imwrite("res_no_resample.bmp", res);
    for (int i = 0; i < H; ++i) {
        printf("rerendering row %d\n", i);
        for (int j = 0; j < W; ++j)
            if (inBoundary(i, j)) {
                Color col;
                for (int k = -RESAMPLE_QUALITY, _; k <= RESAMPLE_QUALITY; ++k)
                    for (int l = -RESAMPLE_QUALITY; l <= RESAMPLE_QUALITY; ++l)
                        col += rayTrace(world.camera.rayCast(i + k / (double)RESAMPLE_QUALITY / 3, j + l / (double)RESAMPLE_QUALITY / 3), RAY_TRACING_DEPTH, 1, false,
                                        _, res) * 255;
                col /= (RESAMPLE_QUALITY * 2 + 1) * (RESAMPLE_QUALITY * 2 + 1);
                res.at<cv::Vec3b>(i, j) = cv::Vec3b(std::min(255.0, col[2]), std::min(255.0, col[1]),
                                                    std::min(255.0, col[0]));
            }
        if (i % 100 == 0) imwrite("tmp.bmp", res);
    }
    output();
    //_color = Scalar(255, 0, 0);
    //drawSegment(Vector3(1.5, 6.3, -1.6), world.light->O, res);
}

Color Renderer::phong(const Collision &collision, const Ray &ray, int &hashId)
{
    Primitive *primitive = collision.primitive;
    hashId *= HASH_BASE;
    hashId += collision.primitive->hashId;
    Color C = world.bgc;
    Color radiance = world.getRadiance(collision.P, collision.N);
    if (radiance.power() < EPS) return C * collision.color;
    hashId += 1. * world.light->hashId * (2 * world.SHADE_QUALITY + 1) * (2 * world.SHADE_QUALITY + 1) * radiance.power();
    //printf("%.3f ", radiance);
    const Vector3 &P = collision.P, L = (world.light->O - P).normalized(), V = -ray.V, N = collision.N;
    Vector3 H = (L + V).normalized();
    C += radiance * (primitive->diff * L.dot(N) + primitive->spec * pow(H.dot(N), PHONG_SEC_POW));
    return collision.color * C;
}
Color Renderer::rayTrace(Ray ray, int depth, Color weight, bool inside, int &hashId, Mat img)
{
#ifdef SHOW_TRACE
    putchar('(');
#endif
    //cout << '[' << wgt << ']';
    if (weight <= EPS || !depth) return Color(0, 0, 0);
    Collision collision = world.findNearestPrimitive(ray);
#ifdef SHOW_TRACE
    if (!collision.crash) putchar(')');
#endif
    if (!collision.crash) return world.bgc * weight;
#ifdef SHOW_TRACE
    cout << '[' << collision.primitive->id << ']';
    img = img.clone();
    drawSegment(ray.O, collision.P, img);
    SHOW_IMG(img);
#endif
    Primitive *primitive = collision.primitive;
    Color res(0, 0, 0);
    int new_hash = hashId;
    res = phong(collision, ray, new_hash);
    res *= weight;
    //if (MAX_DEPTH - depth < 3)
    hashId = new_hash;
#ifdef SHOW_TRACE
    cout << res << " + ";
    _color = Scalar(0,0,255);
    cout << primitive->refr << "*";
#endif
    if (primitive->refr > EPS) res += calcRefr(collision, ray, depth, weight, inside, hashId, img);
#ifdef SHOW_TRACE
    _color = Scalar(0,255,0);
    cout << " + " << primitive-> refl << "*";
#endif
    if (primitive->refl > EPS) res += calcRefl(collision, ray, depth, weight, inside, hashId, img);
#ifdef SHOW_TRACE
    cout << res * 255 ;
    putchar(')');
#endif
    return res;
}

Color Renderer::calcRefl(const Collision &collision, const Ray &ray, int depth, Color weight, bool inside, int &hashId,
                         Mat &img)
{
    Ray newRay = ray.reflected(collision.P, collision.N);
    Primitive *primitive = collision.primitive;
    return rayTrace(newRay, depth - 1, weight * primitive->refl, inside, hashId, img);
}

Color
Renderer::calcRefr(const Collision &collision, const Ray &ray, int depth, const Color &weight, bool inside, int &hashId,
                   Mat &img)
{
    Primitive *primitive = collision.primitive;
    double n = primitive->rindex;
    if (inside) n = 1 / n;
    bool new_inside;
    Ray newRay = ray.refracted(collision.P, collision.N, n, new_inside);
    new_inside ^= inside;
    Vector3 new_weight = weight * primitive->refr;
    if (inside)
        new_weight *= exp(-collision.dist * primitive->absor);
    return rayTrace(newRay, depth - 1, new_weight, new_inside, hashId, img);
}

void Renderer::drawSegment(Vector3 u, Vector3 v, Mat &img, Scalar color)
{
    Ray seg(u, v - u);
    //cout << u << '\t' << v << endl;
    double len = (v - u).len(), intvl = max(0.01, len / 50);
    for (double t = 0; t <= len; t += intvl) {
        Vector3 vec(seg.travel(t));
        Vector3 pos = world.camera.getPixelPos(vec);
        double tmp = (0.1 + 0.9*world.getRadianceWithLight(vec, world.camera.O - vec, world.camera.O));// + 0.7/(world.camera.O - vec).len()*3);
        cv::circle(img, cv::Point(pos[0], pos[1]), 2, _color* tmp/* + (1-tmp)*Scalar(res.at<Vec3b>(min(max(0, (int)pos[1]), H), min(max(0, (int)pos[0]), H)))*/, -1);
    }
}

bool Renderer::inBoundary(int i, int j)
{
    int val = oid.at<int>(i, j);
    for (int k = -1; k <= 1; ++k)
        for (int l = -1; l <= 1; ++l)
            if (i + k >= 0 && i + k < H && j + l >= 0 && j + l < W && oid.at<int>(i + k, j + l) != val) return true;
    return false;
}

