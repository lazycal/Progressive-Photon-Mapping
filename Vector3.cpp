//
// Created by lazycal on 2017/6/11.
//
#include <cassert>
#include "Vector3.h"
#include <cmath>
const double EPS = 1e-5;
const double INF = 1e50;

Vector3 operator + ( const Vector3& A , const Vector3& B ) {
    return Vector3( A.x + B.x , A.y + B.y , A.z + B.z );
}

Vector3 operator - ( const Vector3& A , const Vector3& B ) {
    return Vector3( A.x - B.x , A.y - B.y , A.z - B.z );
}

Vector3 operator * ( const Vector3& A , const double& k ) {
    return Vector3( A.x * k , A.y * k , A.z * k );
}

Vector3 operator / ( const Vector3& A , const double& k ) {
    return Vector3( A.x / k , A.y / k , A.z / k );
}

Vector3 operator * ( const Vector3& A , const Vector3& B ) {
    return Vector3( A.x * B.x , A.y * B.y , A.z * B.z );
}

Vector3& operator += ( Vector3& A , const Vector3& B ) {
    A = A + B;
    return A;
}

Vector3& operator -= ( Vector3& A , const Vector3& B ) {
    A = A - B;
    return A;
}

Vector3& operator *= ( Vector3& A , const double& k ) {
    A = A * k;
    return A;
}

Vector3& operator /= ( Vector3& A , const double& k ) {
    A = A / k;
    return A;
}

Vector3& operator += ( Vector3& A , const double& k ) {
    A = A + k;
    return A;
}

Vector3& operator *= ( Vector3& A , const Vector3& B ) {
    A = A * B;
    return A;
}

Vector3 operator - ( const Vector3& A ) {
    return Vector3( -A.x , -A.y , -A.z );
}

double Vector3::dot(const Vector3 &term) const{
    return x * term.x + y * term.y + z * term.z;
}

Vector3 Vector3::cross(const Vector3 &term) const{
    return Vector3(y * term.z - z * term.y , z * term.x - x * term.z , x * term.y - y * term.x );
}

double Vector3::len2() const{
    return x * x + y * y + z * z;
}

double Vector3::len() const{
    return std::sqrt( x * x + y * y + z * z );
}

double Vector3::dis2(Vector3 &term) const{
    return (term - *this).len2();
}

double Vector3::dis(Vector3 &term) const{
    return (term - *this).len();
}

Vector3 Vector3::normalized() const{
    double module = len();
    if (module < EPS) return Vector3(0, 0, 1);
    return *this / module;
}

double &Vector3::operator[](int idx) {
    if ( idx == 0 ) return x;
    else if ( idx == 1 ) return y;
    else return z;
}

std::ostream &operator<<(std::ostream &os, const Vector3 &vector) {
    os << "(" << vector.x << "," << vector.y << "," << vector.z << ")";
    return os;
}

std::stringstream &operator>>(std::stringstream &fin, Vector3 &vec) {
    fin >> vec.x >> vec.y >> vec.z;
    return fin;
}

Vector3 Vector3::reflected(const Vector3 &N) const{
#ifdef DEBUG
    assert(!sign(N.len() - 1));
#endif
    return -2 * N * dot(N) + *this;
}

bool Vector3::operator<(const Vector3 &rhs) const {
    if (x < rhs.x)
        return true;
    if (rhs.x < x)
        return false;
    if (y < rhs.y)
        return true;
    if (rhs.y < y)
        return false;
    return z < rhs.z;
}

bool Vector3::operator>(const Vector3 &rhs) const {
    return rhs < *this;
}

bool Vector3::operator<=(const Vector3 &rhs) const {
    return !(rhs < *this);
}

bool Vector3::operator>=(const Vector3 &rhs) const {
    return !(*this < rhs);
}

bool Vector3::operator<(double rhs) const {
    return x < rhs && y < rhs && z < rhs;
}

Vector3 Vector3::refracted(const Vector3 &N, double n, bool &isRefracted) const{
#ifdef DEBUG
    assert(!sign(N.len() - 1));
    assert(!sign(len() - 1));
#endif
    Vector3 V = -*this;
    double cos1 = N.dot(V), sin22 = (1 - cos1 * cos1)/n/n;
    isRefracted = 0;
    if (sin22 > 1) return reflected(N);
    double cos2 = sqrt(1 - sin22);
    isRefracted = 1;
    return -V/n - (cos2 - cos1/n) * N;
}

Vector3 exp(const Vector3 &vec) {
    return Vector3(exp(vec.x ), exp(vec.y), exp(vec.z));
}

bool Vector3::operator==(const Vector3 &rhs) const {
    return !sign(x - rhs.x) &&
           !sign(y - rhs.y) &&
           !sign(z - rhs.z);
}

bool Vector3::equal(const Vector3 &rhs, double eps) const {
    return !sign(x - rhs.x, eps) &&
           !sign(y - rhs.y, eps) &&
           !sign(z - rhs.z, eps);
}

bool Vector3::operator!=(const Vector3 &rhs) const {
    return !(rhs == *this);
}

Vector3 Vector3::transformedToNewAxis(Vector3 dx, Vector3 dy, Vector3 dz) const
{
#ifdef DEBUG
    assert(!sign(dx.len()-1));assert(!sign(dy.len()-1));assert(!sign(dz.len()-1));
#endif
    return Vector3(dot(dx), dot(dy), dot(dz));
}

const double &Vector3::operator[](int idx) const
{
    if ( idx == 0 ) return x;
    else if ( idx == 1 ) return y;
    else return z;
}

double Vector3::power() const
{
    return (x + y + z) / 3;
}

Vector3 Vector3::getAnVerticalVector() const
{
#ifdef DEBUG
    assert(!sign(len() - 1));
#endif
    Vector3 res = dot(Vector3(0, 0, 1));
    if (res == 0) res = Vector3(1, 0, 0);
    return res;
}

Vector3 Vector3::rotated(const Vector3 &axis, double theta) const
{
// copied from http://blog.csdn.net/zsyzgu/article/details/46592041
    Vector3 ret;
    double cost = cos( theta );
    double sint = sin( theta );
    ret.x += x * ( axis.x * axis.x + ( 1 - axis.x * axis.x ) * cost );
    ret.x += y * ( axis.x * axis.y * ( 1 - cost ) - axis.z * sint );
    ret.x += z * ( axis.x * axis.z * ( 1 - cost ) + axis.y * sint );
    ret.y += x * ( axis.y * axis.x * ( 1 - cost ) + axis.z * sint );
    ret.y += y * ( axis.y * axis.y + ( 1 - axis.y * axis.y ) * cost );
    ret.y += z * ( axis.y * axis.z * ( 1 - cost ) - axis.x * sint );
    ret.z += x * ( axis.z * axis.x * ( 1 - cost ) - axis.y * sint );
    ret.z += y * ( axis.z * axis.y * ( 1 - cost ) + axis.x * sint );
    ret.z += z * ( axis.z * axis.z + ( 1 - axis.z * axis.z ) * cost );
    return ret;
}

Vector3 Vector3::randomVector()
{
    double x, y, z;
    do {
        x = 2. * randreal() - 1;
        y = 2. * randreal() - 1;
        z = 2. * randreal() - 1;
    } while (x*x+y*y+z*z>1 || x*x+y*y+z*z < EPS);
    return Vector3(x, y, z).normalized();
}
