#include <iostream>
#include <sys/time.h>
#include "PPMRenderer.h"
#include "Plane.h"
#include "Polynomial.h"

using namespace std;
int main2()
{
//    Plane plane;
//    plane.N = Vector3(0,0,1);
//    plane.r = -2;
//    plane.init();
//    Ray ray(Vector3(0,0,-3), Vector3(1,1,0));
//    cout << plane.collide(ray);
//    Vector3 a(sqrt(3),1,0);
//    a = -a.normalized();
//    bool tmp = false;
//    cout << a.refracted(Vector3(1,0,0),0.5,tmp) << ' ' << tmp << endl;
//    Polynomial f(3, -5);
//    f.c.push_back(1.5);
//    for (auto i : f.roots(0, 0))
//        printf("%.10f,%.10f\n", i.real(), f(i.real()));

}
double dtime()
{
    double tseconds = 0.0;
    struct timeval mytime;
    gettimeofday(&mytime,(struct timezone*)0);
    tseconds = (double)(mytime.tv_sec+mytime.tv_usec*1.0e-6);
    return (tseconds);
}
int main(int, const char * const [])
{
    double st = dtime();
    PPMRenderer renderer("../../scenes/scene_plate.txt", "res");
    //std::cout<<renderer.world.camera<<std::endl;
    //Renderer renderer("input.txt", "res");
    renderer.run();
    cout << (dtime() - st) << "s\n";
    return 0;

}

//
// File trailer for main.cpp
//
// [EOF]
//
