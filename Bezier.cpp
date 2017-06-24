//
// Created by lazycal on 2017/6/15.
//

#include "Bezier.h"

Color Bezier::getColor(const Vector3 &P)
{
    return Primitive::getColor(P);
}

Collision Bezier::collide(const Ray &o_ray)
{
#ifdef DEBUG
    static double tot_time = 0, eq_time = 0;
    double st = clock();
    assert(o_ray.V.x == o_ray.V.x && o_ray.V.y == o_ray.V.y && o_ray.V.z == o_ray.V.z);
#endif
    if (!o_ray.crash(bbox)) return Collision(this);
    Vector3 O = o_ray.O - this->P;
    Vector3 V = o_ray.V.transformedToNewAxis(Dx, Dy, Dz);
    O = O.transformedToNewAxis(Dx, Dy, Dz);
    Ray new_ray(O, V);
    //Solve t,u in the equations
    // P(t).x^2 = (O.x+u*V.x)^2 + (O.y+u*V.y)^2
    // P(t).y   = O.z + u*V.z;
    Polynomial eq;
    std::vector<std::complex<double>> ts;
#ifdef DEBUG
    double st_eq = clock();
#endif
    if (!sign(V.z)) {eq = (py - O.z); eq.roots(ts);}
    else {
        Polynomial nu = (py - O.z) / V.z;
        eq = pow(px, 2) - pow(O.x + nu * V.x, 2) - pow(O.y + nu * V.y, 2);
        eq.roots(ts);
    }
#ifdef DEBUG
    eq_time += clock() - st_eq;
#endif

    double ans_t, ans_theta;
    Collision coll(this); coll.dist = INF;
#ifdef DEBUG
    assert(!(coll.dist < INF));
#endif
    for (auto c : ts) {
        if (sign(c.imag())) continue;
        double t = c.real(), nu = -1;
        //if ((sign(V.z) && sign((t - 1)/fabs(V.z), 1) > 0 || sign(t/fabs(V.z) > 0), 1) || sign(t - 1) > 0 || sign(t) < 0) continue;
        if (sign(t - 1) > 0 || sign(t) < 0) continue;
        t = max(0., min(1., t));
        if (sign(eq(t), 1e-3)) continue;
        if (sign(V.z)) nu = (py(t) - O.z) / V.z;
        else {
            Vector3 center(0, 0, O.z);
            double r = px(t);
            if (sign(new_ray.dis2(center) - r * r) >= 0)
                continue;
            Vector3 l = center - new_ray.O;
            double d2 = l.cross(new_ray.V).len2() / new_ray.V.len2();
            double tp = l.dot(new_ray.V); // > 0 <90 ; < 0 > 90
            double x = sqrt(r * r - d2);
            if (sign(l.len2() - r * r) <= 0) nu = tp + x;
            else nu = tp - x;
            assert(nu > 0);
        }
        //nu = (py(t) - O.z) / V.z;
        if (sign(nu, 1e-3) <= 0) continue;//TODO: ???
//        double theta;
//        Vector3 nP = new_ray.travel(nu);
//        if (!sign(nP.x) && !sign(nP.y)) theta = 0;
//        else theta = atan2(nP.y, nP.x);
//        if( || getPoint(t, theta) != coll.P) continue;
#ifdef DEBUG
        assert(std::abs(pow(px(t),2) - new_ray.travel(nu).x * new_ray.travel(nu).x
        - new_ray.travel(nu).y * new_ray.travel(nu).y) < 0.1);
        assert(std::abs(py(t) - new_ray.travel(nu).z) < 0.1);
#endif
        if (coll.dist > nu) coll.dist = nu, ans_t = t;
    }
    if (coll.dist < INF) {
        coll.crash = true;
        Vector3 nP = new_ray.travel(coll.dist);
        //coll.N = getN(ans_t, theta);
        Vector3 p_theta(-nP.y, nP.x, 0);
        double dpx_v = dpx(ans_t), dpy_v = dpy(ans_t), px_v = px(ans_t);
        Vector3 p_t(nP.x / px_v * dpx_v, nP.y / px_v * dpx_v, dpy_v);
        coll.N = p_t.cross(p_theta).normalized();
        if (new_ray.V.dot(coll.N) > 0) coll.N = -coll.N;
        Vector3 ox(1, 0, 0), oy(0, 1, 0), oz(0, 0, 1);
        ox = ox.transformedToNewAxis(Dx, Dy, Dz);
        oz = oz.transformedToNewAxis(Dx, Dy, Dz);
        oy = oy.transformedToNewAxis(Dx, Dy, Dz);
        coll.N = coll.N.transformedToNewAxis(ox, oy, oz);
        coll.P = o_ray.travel(coll.dist);
        if (!sign(nP.x) && !sign(nP.y)) ans_theta = 0;
        else {
            ans_theta = atan2(nP.y, nP.x) + PI*(px_v < 0);
            if (ans_theta > PI) ans_theta -= 2 * PI;
        }
        coll.color = getColor(ans_t, ans_theta);
    }
#ifdef DEBUG
    if (coll.crash) {
        assert(coll.N.x == coll.N.x && coll.N.y == coll.N.y && coll.N.z == coll.N.z);
        assert(getPoint(ans_t, ans_theta).equal(coll.P, 1e-1));
    }
//    tot_time += clock() - st;
//    if (int(tot_time / CLOCKS_PER_SEC) % 5 == 0)
//        printf("tot_time=%.3fs eq_time=%.3fs ratio=%.3f\n", tot_time / CLOCKS_PER_SEC, eq_time / CLOCKS_PER_SEC, eq_time / tot_time);
#endif
    return coll;
}

void Bezier::input(const std::string &var, std::stringstream &ss)
{
    if (var == "P=") ss >> P;
    else if (var == "Dz=") ss >> Dz;
    else if (var == "S=") ss >> S;
    else if (var == "control_points=") {
        int n;
        ss >> n;
        for (int i = 0; i < n; ++i) {
            Vector3 p;
            ss >> p.x >> p.y;
            control_points.push_back(p);
        }
    }
    else Primitive::input(var, ss);
}

std::pair<Polynomial, Polynomial> Bezier::P2d(int l, int n)
{
    if (n == 1)
        return std::pair<Polynomial, Polynomial>(control_points[l][0], control_points[l][1]);
    std::pair<Polynomial, Polynomial> lhs = P2d(l, n - 1), rhs = P2d(l + 1, n - 1);
    return std::pair<Polynomial, Polynomial>(Polynomial(1, -1) * lhs.first  + Polynomial(0, 1) * rhs.first,
                                             Polynomial(1, -1) * lhs.second + Polynomial(0, 1) * rhs.second);
}
//Vector3 Bezier::P(double u, double v)
//{
//    double theta = v * 2 * PI;
//    Point p2d = P2d(u, 0, n);
//    return Point(p2d.x * cos(theta), p2d.x * sin(theta), p2d.y);
//}
void Bezier::init()
{
    printf("initializing Bezier\n");
    Dz = Dz.normalized();
    if (!sign(Dz.x) && !sign(Dz.y - 1) && !sign(Dz.z)) Dx = Vector3(1, 0, 0);
    else Dx = Vector3(0, 1, 0).cross(Dz).normalized();
    Dy = Dz.cross(Dx);
    for (auto &i :control_points) i /= S;
    std::pair<Polynomial, Polynomial> tmp = P2d(0, control_points.size());
    px = tmp.first; py = tmp.second;
    dpx = px.derivative(); dpy = py.derivative();
    printf("Test for Bezier: P(%.3f)=(%.3f, %.3f)\n", 0., px(0), py(0));
    printf("Test for Bezier: P(%.3f)=(%.3f, %.3f)\n", 0.5, px(0.5), py(0.5));
    printf("Test for Bezier: P(%.3f)=(%.3f, %.3f)\n", 1., px(1), py(1));
    std::cout << "Bezier points:\n";
    for (float u = 0.0f, i = 0, du = 0.01; u <= 1.0f; u += du, i++) {
        for (float v = 0.0f, j = 0, dv = 0.01; v <= 1.0f; v += dv, j++) {
            bbox.include(getPoint(u, v * 2 * PI));
        }
    }
    std::cout << bbox << std::endl;
}

//Vector3 Bezier::getN(double t, double theta)
//{
//    Vector3 p = getPoint(t, theta);
//
//}
//
Vector3 Bezier::getPoint(double t, double theta)
{
    return P + py(t) * Dz + Dx * px(t) * cos(theta) + Dy * px(t) * sin(theta);
}

std::ostream &operator<<(std::ostream &os, const Bezier &bezier)
{
    os << static_cast<const Primitive &>(bezier) << " bbox: " << bezier.bbox << " P: " << bezier.P << " S: " << bezier.S
       ;//<< " control_points: " << bezier.control_points;
    return os;
}

Color Bezier::getColor(double t, double theta)
{
    if (texture.data == NULL) return color;
    double x = t, y = theta / PI / 2 + 0.5;
#ifdef DEBUG
    assert(sign(x) >= 0 && sign(y) >=0 && sign(x - 1) <= 0 && sign(y - 1) <= 0);
#endif
    return texture.getColor(x, y);
}

void Bezier::generateMeshes(std::vector<Vector3> &points, std::vector<int4> &meshes)
{
    float du = 0.01f, dv = 0.01f;             // 自定义密度
    int nu = 1 / du + 1, nv = 1 / dv + 1, offset = points.size();   // 密度应该“除”得整
    int pid = offset;                            // 点序号
    for (float u = 0.0f, i = 0; u <= 1.0f; u += du, i++) {
        for (float v = 0.0f, j = 0; v <= 1.0f; v += dv, j++) {
            points.push_back(getPoint(u, v * PI * 2));          // 写递归或者DP搞定函数P的计算
            pid++;                              // OBJ格式网格序号从1开始
            if (i != 0 && j != 0) {
                //printf("f %d %d %d %d\n", pid-nv-1, pid-nv, pid, pid-1);
                meshes.push_back(int4(pid-nv-1, pid-nv, pid, pid-1));
            }
        }
    }
}
