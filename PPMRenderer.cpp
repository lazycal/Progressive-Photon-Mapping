//
// Created by lazycal on 2017/6/20.
//

#include "PPMRenderer.h"
#include "Bezier.h"
//#define SHOW_TRACE

using cv::Mat;
using namespace std;


PPMRenderer::PPMRenderer(std::string input_file, std::string output_file) : world(input_file), output_file(output_file)
{
    MAX_THREADS = 4;
    ITERS = 100;
    PHOTONS_PER_ITER = 1000000;
    PHOTON_TRACING_DEPTH = 20;
    HASH_BASE = 92797;
    MOD = 1000000009;
    RESAMPLE_QUALITY = 1;
    H = world.camera.H;
    W = world.camera.W;
    oid = Mat(H, W, CV_32S, Scalar::all(0));
    PHONG_SEC_POW = 50;
    RAY_TRACING_DEPTH = 20;
    res.create(H, W, CV_8UC3);
	rt_res.create(H, W, CV_8UC3);
    rtHitMap.resize(H * W);
    FILE* fout = fopen((output_file + ".obj").c_str(), "w");
    for (auto i: world.points) fprintf(fout, "v %.10f %.10f %.10f\n", i.x, i.y, i.z);
    for (auto i: world.meshes) fprintf(fout, "f %d %d %d %d\n", i.data[0], i.data[1], i.data[2], i.data[3]);
    fclose(fout);
}

void PPMRenderer::output(string suf)
{
    assert(res.rows > 0 && res.cols > 0);
    imwrite(output_file+suf+".bmp", res);
}

void PPMRenderer::run()
{//518 472
    std::vector<std::thread*> threads;
    rt_i = rt_j = 0;
    for (int _ = 0; _ < MAX_THREADS; ++_) {
        std::thread *td = new std::thread([this]() {
            while (1) {
                int i;
                mt.lock();
                nextRow(i);
                mt.unlock();
                if (i >= H) return;
                printf("tracing row %d\n", i);
                for (int j = 0; j < W; ++j) {
#ifdef SHOW_TRACE
                    printf("\nrendering row %d col %d\n", i, j);
                            _color = Scalar(255, 0, 0);
#endif
                    Color vec = rayTrace(world.camera.rayCast(i, j), RAY_TRACING_DEPTH, 1, false, oid.at<int>(i, j),
                                         rtHitMap[i * W + j],
                                         rt_res) * 255;
                    //mt.lock();
                    rt_res.at<Vec3b>(i, j) = Vec3b(std::min(255.0, vec[2]), std::min(255.0, vec[1]),
                                                   std::min(255.0, vec[0]));
                    //mt.unlock();
#ifdef SHOW_TRACE
                    cout << res.at<cv::Vec3b>(i, j) << endl;
#endif
                }
            }
        });
        threads.push_back(td);
    }
    for (auto & td : threads) {
        td->join();
        delete td;
    }
    threads.clear();
    imwrite(output_file+"_rt_noresampled.bmp", rt_res);
    rt_i = rt_j = 0;
    for (int _ = 0; _ < MAX_THREADS; ++_) {
        std::thread *td = new std::thread([this]() {
            while (1) {
                int i;
                mt.lock();
                nextRow(i);
                mt.unlock();
                if (i >= H) return;
                printf("retracing row %d\n", i);
                for (int j = 0; j < W; ++j)
                if (inBoundary(i, j)) {
                    Color col;
                    rtHitMap[i * W + j].clear();
                    for (int k = -RESAMPLE_QUALITY, _; k <= RESAMPLE_QUALITY; ++k)
                        for (int l = -RESAMPLE_QUALITY; l <= RESAMPLE_QUALITY; ++l)
                            col += rayTrace(world.camera.rayCast(i + k / (double) RESAMPLE_QUALITY / 3,
                                                                 j + l / (double) RESAMPLE_QUALITY / 3),
                                            RAY_TRACING_DEPTH,
                                            1. / (RESAMPLE_QUALITY * 2 + 1) / (RESAMPLE_QUALITY * 2 + 1), false,
                                            _, rtHitMap[i * W + j], rt_res) * 255;
//                    mt.lock();
                    rt_res.at<Vec3b>(i, j) = Vec3b(std::min(255.0, col[2]), std::min(255.0, col[1]),
                                                   std::min(255.0, col[0]));
//                    mt.unlock();
                }
            }
        });
        threads.push_back(td);
    }
    for (auto & td : threads) {
        td->join();
        delete td;
    }
    threads.clear();
    imwrite(output_file+"_rt.bmp", rt_res);
    for (int iter = 1; iter <= ITERS; ++iter) {
        printf("photon iter=%d\n", iter);
        photonMap.clear();
        for (int j = 0; j < PHOTONS_PER_ITER; ++j)
            photonTrace(world.light->emitPhoton(), PHOTON_TRACING_DEPTH, false);
        printf("photons=%d\n", photonMap.size());
        photonMap.build();
        //SHOW_IMG(res);
        threads.clear();
        rt_i = rt_j = 0;
        int i;
        for (int _ = 0; _ < MAX_THREADS; ++_) {
            std::thread *td = new std::thread([this, iter]() {
                while (1) {
                    int i;
                    mt.lock();
                    nextRow(i);
                    mt.unlock();
                    if (i >= H) return;
                    printf("photon rendering row %d\n", i);
                    std::vector<Vec3b> col(W);
                    for (int j = 0; j < W; ++j) {
                        Vector3 color;
                        for (auto &hitPoint : rtHitMap[i * W + j]) {
                            photonMap.update(hitPoint);
                            color += hitPoint.wgt * hitPoint.color * hitPoint.flux / hitPoint.R / hitPoint.R;
                        }
                        color = color * 255 / PI / PHOTONS_PER_ITER / iter;
                        //Vec3b rt_col = rt_res.at<Vec3b>(i, j);
                        col[j] = Vec3b(std::min(color[2], 255.),
                                       std::min(color[1], 255.),
                                       std::min(color[0], 255.));
                        //res.at<Vec3b>(i, j) = rt_col;
//                Vec3b(std::min(rt_col[2],255),
//                                            std::min(rt_col[1],255),
//                                            std::min(rt_col[0],255));
                    }
                    mt.lock();
                    for (int j = 0; j < W; ++j)
                        res.at<Vec3b>(i, j) = col[j];
                    mt.unlock();
                }
            });
            threads.push_back(td);
        }
        for (auto & td : threads) {
            td->join();
            delete td;
        }
        char suf[10];
        sprintf(suf, "-%d", iter);
        output(suf);
    }
/*
    for (int i = 0; i < H; ++i) {
        printf("tracing row %d\n", i);
        for (int j = 0; j < W; ++j) {
#ifdef SHOW_TRACE
            printf("\nrendering row %d col %d\n", i, j);
            _color = Scalar(255, 0, 0);
#endif
            Color vec = rayTrace(world.camera.rayCast(i, j), RAY_TRACING_DEPTH, 1, false, oid.at<int>(i, j), rtHitMap[i * W + j],
                     rt_res) * 255;
            rt_res.at<Vec3b>(i, j) = Vec3b(std::min(255.0, vec[2]), std::min(255.0, vec[1]), std::min(255.0, vec[0]));
#ifdef SHOW_TRACE
            cout << res.at<cv::Vec3b>(i, j) << endl;
#endif
        }
    }
    imwrite(output_file+"_rt_noresampled.bmp", rt_res);
    for (int i = 0; i < H; ++i) {
        printf("retracing row %d\n", i);
        for (int j = 0; j < W; ++j)
            if (inBoundary(i, j)) {
                Color col;
                rtHitMap[i * W + j].clear();
                for (int k = -RESAMPLE_QUALITY, _; k <= RESAMPLE_QUALITY; ++k)
                    for (int l = -RESAMPLE_QUALITY; l <= RESAMPLE_QUALITY; ++l)
                        col += rayTrace(world.camera.rayCast(i + k / (double) RESAMPLE_QUALITY / 3,
                                                      j + l / (double) RESAMPLE_QUALITY / 3), RAY_TRACING_DEPTH,
                                 1. / (RESAMPLE_QUALITY * 2 + 1) / (RESAMPLE_QUALITY * 2 + 1), false,
                                 _, rtHitMap[i * W + j], rt_res) * 255;
                rt_res.at<Vec3b>(i, j) = Vec3b(std::min(255.0, col[2]), std::min(255.0, col[1]), std::min(255.0, col[0]));
            }
    }
    imwrite(output_file+"_rt.bmp", rt_res);
    for (int iter = 1; iter <= ITERS; ++iter) {
        printf("photon iter=%d\n", iter);
        photonMap.clear();
        for (int j = 0; j < PHOTONS_PER_ITER; ++j)
            photonTrace(world.light->emitPhoton(), PHOTON_TRACING_DEPTH, false);
        printf("photons=%d\n", photonMap.size());
        photonMap.build();
        //SHOW_IMG(res);
        for (int i = 0; i < H; ++i)
            for (int j = 0; j < W; ++j) {
                Vector3 color;
                for (auto &hitPoint : rtHitMap[i * W + j]) {
                    photonMap.update(hitPoint);
                    Primitive *primitive = hitPoint.primitive;
                    color += hitPoint.wgt * hitPoint.color * hitPoint.flux/hitPoint.R/hitPoint.R;
                }
                color = color * 255 /PI / PHOTONS_PER_ITER / iter;
                Vec3b rt_col = rt_res.at<Vec3b>(i, j);
                res.at<Vec3b>(i, j) = Vec3b(std::min(color[2],255.),
                                            std::min(color[1],255.),
                                            std::min(color[0],255.));
                //res.at<Vec3b>(i, j) = rt_col;
//                Vec3b(std::min(rt_col[2],255),
//                                            std::min(rt_col[1],255),
//                                            std::min(rt_col[0],255));
            }
        char suf[10];
        sprintf(suf, "-%d", iter);
        output(suf);
    }
    //_color = Scalar(255, 0, 0);
    //drawSegment(Vector3(1.5, 6.3, -1.6), world.light->O, res);
    */
}

Color PPMRenderer::phong(const Collision &collision, const Ray &ray, int &hashId)
{
    Primitive *primitive = collision.primitive;
    long long new_hash = 0;
    new_hash = 1ll * hashId * HASH_BASE;
    new_hash += collision.primitive->hashId;
    Color C = world.bgc;
    Color radiance = world.getRadiance(collision.P, collision.N);
    if (radiance.power() < EPS) {hashId = new_hash % MOD; return C * collision.color;}
    new_hash += 1. * world.light->hashId * (2 * world.SHADE_QUALITY + 1) * (2 * world.SHADE_QUALITY + 1) * radiance.power();
    hashId = new_hash % MOD;
    //printf("%.3f ", radiance);
    const Vector3 &P = collision.P, L = (world.light->O - P).normalized(), V = -ray.V, N = collision.N;
    Vector3 H = (L + V).normalized();
    C += radiance * (primitive->diff * L.dot(N) + primitive->spec * pow(H.dot(N), PHONG_SEC_POW));
    return collision.color * C;
}

Color
PPMRenderer::rayTrace(Ray ray, int depth, Color weight, bool inside, int &hashId, std::vector<RtHitPoint> &hitPoints,
                      Mat img)
{
#ifdef SHOW_TRACE
    putchar('(');
#endif
    //cout << '[' << wgt << ']';
    if (weight < EPS || !depth) return Color();
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
    if (primitive->diff + primitive->spec > EPS)
        hitPoints.push_back(RtHitPoint(collision.P, collision.N, weight, world.camera.getPixelLength(collision.P),
                                       collision.primitive, collision.color));
    Color res(0);
    int new_hash = hashId;
    res = phong(collision, ray, new_hash) * weight;
    //if (MAX_DEPTH - depth < 3)
    hashId = new_hash;
#ifdef SHOW_TRACE
    cout << res << " + ";
    _color = Scalar(0,0,255);
    cout << primitive->refr << "*";
#endif
    if (primitive->refr > EPS) res += calcRefr(collision, ray, depth, weight, inside, hashId, hitPoints, img);
#ifdef SHOW_TRACE
    _color = Scalar(0,255,0);
    cout << " + " << primitive-> refl << "*";
#endif
    if (primitive->refl > EPS) res += calcRefl(collision, ray, depth, weight, inside, hashId, hitPoints, img);
#ifdef SHOW_TRACE
    cout << res * 255 ;
    putchar(')');
#endif
    return res;
}

Color
PPMRenderer::calcRefl(const Collision &collision, const Ray &ray, int depth, Color weight, bool inside, int &hashId,
                      std::vector<RtHitPoint> &hitPoints, Mat &img)
{
    Ray newRay = ray.reflected(collision.P, collision.N);
    Primitive *primitive = collision.primitive;
    return rayTrace(newRay, depth - 1, weight * primitive->refl, inside, hashId, hitPoints, img);
}

Color
PPMRenderer::calcRefr(const Collision &collision, const Ray &ray, int depth, const Color &weight, bool inside,
                      int &hashId, std::vector<RtHitPoint> &hitPoints, Mat &img)
{
    Primitive *primitive = collision.primitive;
    double n = primitive->rindex;
    if (inside && typeid(*primitive) != typeid(Bezier)) n = 1 / n;
    bool new_inside;
    Ray newRay = ray.refracted(collision.P, collision.N, n, new_inside);
    if (typeid(*primitive) == typeid(Bezier)) new_inside = 0;
    new_inside ^= inside;
    Vector3 new_weight = weight * primitive->refr;
    if (inside && typeid(*primitive) != typeid(Bezier))
        new_weight *= exp(-collision.dist * primitive->absor);
    return rayTrace(newRay, depth - 1, new_weight, new_inside, hashId, hitPoints, img);
}

void PPMRenderer::drawSegment(Vector3 u, Vector3 v, Mat &img, Scalar color)
{
    Ray seg(u, v - u);
    //cout << u << '\t' << v << endl;
    double len = (v - u).len(), intvl = max(0.01, len / 50);
    for (double t = 0; t <= len; t += intvl) {
        Vector3 vec(seg.travel(t));
        Vector3 pos = world.camera.getPixelPos(vec);
        double tmp = (0.1 + 0.9 * world.getRadianceWithLight(vec, world.camera.O - vec,
                                                             world.camera.O));// + 0.7/(world.camera.O - vec).len()*3);
        cv::circle(img, cv::Point(pos[0], pos[1]), 2, color *
                                                      tmp/* + (1-tmp)*Scalar(res.at<Vec3b>(min(max(0, (int)pos[1]), H), min(max(0, (int)pos[0]), H)))*/,
                   -1);
    }
}

bool PPMRenderer::inBoundary(int i, int j)
{
    int val = oid.at<int>(i, j);
    for (int k = -1; k <= 1; ++k)
        for (int l = -1; l <= 1; ++l)
            if (i + k >= 0 && i + k < H && j + l >= 0 && j + l < W && oid.at<int>(i + k, j + l) != val) return true;
    return false;
}

void PPMRenderer::photonTrace(const Photon &photon, int depth, bool inside)
{
#ifdef SHOW_TRACE
    putchar('(');
#endif
    //cout << '[' << wgt << ']';
    Ray ray = photon.ray;
    if (photon.flux * 255 <= 0.1 || !depth) return;
    Collision collision = world.findNearestPrimitive(ray);
#ifdef SHOW_TRACE
    if (!collision.crash) putchar(')');
#endif
    if (!collision.crash) return;
    Primitive *primitive = collision.primitive;
#ifdef SHOW_TRACE
    cout << '[' << collision.primitive->id << ']';
    img = img.clone();
    drawSegment(ray.O, collision.P, img);
    SHOW_IMG(img);
    cout << res << " + ";
    _color = Scalar(0,0,255);
    cout << primitive->refr << "*";
#endif
    double ksi = randreal();
    //cout << "depth=" << depth << '\n';
    if (primitive->diff + primitive->spec > EPS) {
        Photon newPhoton = photon;
        newPhoton.ray.O = collision.P;
        Vector3 pos = world.camera.getPixelPos(newPhoton.ray.O);
        //if (!(pos.x < -0.1 * W || pos.x > W * 1.1 || pos.y < -0.1 * H || pos.y > H * 1.1)) {
            photonMap.addPhoton(newPhoton);
            //drawSegment(collision.P, collision.P, res, Scalar(0, 255, 0));
            //SHOW_IMG(res);
        //}
    }

    if (primitive->diff + primitive->spec > EPS && calcPhotonDiff(collision, photon, depth, inside, ksi)) return;
    if (primitive->refr > EPS && calcPhotonRefr(collision, photon, depth, inside, ksi)) return;
#ifdef SHOW_TRACE
    _color = Scalar(0,255,0);
    cout << " + " << primitive-> refl << "*";
#endif
    if (primitive->refl > EPS && calcPhotonRefl(collision, photon, depth, inside, ksi)) return;
#ifdef SHOW_TRACE
    cout << res * 255 ;
    putchar(')');
#endif

}

bool PPMRenderer::calcPhotonRefr(const Collision &collision, const Photon &photon, int depth, bool inside, double &ksi)
{
    Primitive *primitive = collision.primitive;
    double n = primitive->rindex;
    if (inside && typeid(*primitive) != typeid(Bezier)) n = 1 / n;
    Color c = primitive->refr;
    bool new_inside;
    Photon newPhoton = photon.refracted(collision.P, collision.N, n, new_inside);
    if (inside && typeid(*primitive) != typeid(Bezier)) c *= exp(- collision.dist * primitive->absor);
    if (c.power() <= ksi) {
        ksi -= c.power();
        return false;
    }
    if (typeid(*primitive) == typeid(Bezier)) new_inside = 0;
    new_inside ^= inside;
    newPhoton.flux = newPhoton.flux * c / c.power();
    photonTrace(newPhoton, depth - 1, new_inside);
    return true;
}

bool PPMRenderer::calcPhotonRefl(const Collision &collision, const Photon &photon, int depth, bool inside, double &ksi)
{
    Color c = collision.primitive->refl*collision.color;
    if (c.power() <= ksi) {
        ksi -= c.power();
        return false;
    }
    Photon newPhoton = photon.reflected(collision.P, collision.N);
    newPhoton.flux = newPhoton.flux * c / c.power();
    photonTrace(newPhoton, depth - 1, inside);
    return true;
}

bool PPMRenderer::calcPhotonDiff(const Collision &collision, const Photon &photon, int depth, bool inside, double &ksi)
{
    Color c = (collision.primitive->diff + collision.primitive->spec)*collision.color;
    if (c.power() <= ksi) {
        ksi -= c.power();
        return false;
    }
    Photon newPhoton = photon.diffused(collision.P, collision.N);
    newPhoton.flux = newPhoton.flux * c / c.power();
    photonTrace(newPhoton, depth - 1, inside);
    return true;
}

void PPMRenderer::multithreadRayTrace()
{
}

void PPMRenderer::nextRow(int &i)
{
    i = rt_i++;
}

