//
// Created by lazycal on 2017/6/11.
//

#ifndef RAY_TRACING_VECTOR_H
#define RAY_TRACING_VECTOR_H

#include <sstream>
#include <iostream>
#include <cassert>
#include "header.h"
#define DEBUG
const double PI = acos(-1);
extern const double EPS;
extern const double INF;
inline int sign(double x, double eps = EPS){return x < -eps ? -1 : (x > eps);}
inline double sqr(double x) {return x * x;}
inline double randreal(){ return (1.0 * rand() * (RAND_MAX + 1.) + rand()) / ((RAND_MAX + 1.) * (RAND_MAX + 1.) - 1); }

struct int4
{
    int data[4];
    int4(){}
    int4(int x, int y, int z, int k)
    {
        data[0] = x;
        data[1] = y;
        data[2] = z;
        data[3] = k;
    }
};
struct Vector3 {
    double x, y, z;
    Vector3( double X , double Y , double Z ) : x( X ) , y( Y ) , z( Z ) {}
    Vector3( double X = 0 ) : x( X ) , y( X ) , z( X ) {}
    template <class T>
    Vector3( const Vec<T, 3> & vec ) : x(vec[0]), y(vec[1]), z(vec[2]) {}
    ~Vector3() {}

    bool operator<(const Vector3 &rhs) const;

    bool operator>(const Vector3 &rhs) const;

    bool operator<=(const Vector3 &rhs) const;

    bool operator>=(const Vector3 &rhs) const;

    bool operator<(double rhs) const;

    bool operator==(const Vector3 &rhs) const;

    bool operator!=(const Vector3 &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const Vector3 &vector);
    friend std::stringstream &operator>>(std::stringstream &, Vector3&);
    friend Vector3 operator + ( const Vector3& , const Vector3& );
    friend Vector3 operator - ( const Vector3& , const Vector3& );
    friend Vector3 operator * ( const Vector3& , const double& );
    friend Vector3 operator / ( const Vector3& , const double& );
    friend Vector3 operator * ( const Vector3& , const Vector3& );
    friend Vector3& operator += ( Vector3& , const Vector3& );
    friend Vector3& operator -= ( Vector3& , const Vector3& );
    friend Vector3& operator *= ( Vector3& , const double& );
    friend Vector3& operator /= ( Vector3& , const double& );
    friend Vector3& operator *= ( Vector3& , const Vector3& );
    friend Vector3 operator - ( const Vector3& );
    friend Vector3 exp (const Vector3&);


    double& operator [] (int idx);
    const double& operator [] (int idx)const;
    double dot(const Vector3 &) const;
    Vector3 cross(const Vector3 &) const;
    double len2() const;
    double len() const;
    double dis2(Vector3 &) const;
    double dis(Vector3 &) const;
    Vector3 normalized() const;
//    void AssRandomVector();
    Vector3 getAnVerticalVector() const;
//    bool IsZeroVector();
//    //void Input( std::stringstream& fin );
    Vector3 reflected( const Vector3 &N ) const;
    Vector3 refracted( const Vector3 &N , double n , bool &isRefracted) const;
//    Vector3 Diffuse();
    Vector3 rotated(const Vector3 &Dz, double theta) const;
//    void Output();
    Vector3 transformedToNewAxis(Vector3 x, Vector3 y, Vector3 z)const;

    bool equal(const Vector3 &rhs, double eps) const;

    double power() const;

    static Vector3 randomVector();
};
typedef Vector3 Color;


#endif //RAY_TRACING_VECTOR_H
