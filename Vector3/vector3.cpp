//#pragma once
#define _USE_MATH_DEFINES
#include <cstring>
#include "vector3.h"
#include <algorithm>
#include <cmath>

vector3 :: vector3 (float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}
vector3 :: vector3()
{

}

vector3 vector3 :: self()
{
    vector3 self(X(), Y(), Z());
    return self;
}
void vector3 :: setX(float x)
{
    this->x = x;
}
void vector3 :: setY(float y)
{
    this->y = y;
}
void vector3 :: setZ(float z)
{
    this->z = z;
}
float vector3 :: X() const
{
    return x;
}
float vector3 :: Y() const
{
    return y;
}
float vector3 :: Z() const
{
    return z;
}
vector3 vector3 :: operator+ (vector3 const &other) const
{
    vector3 tmp(X() + other.X(), Y() + other.Y(), Z() + other.Z());
    return tmp;
}
vector3 vector3 :: operator-(vector3 const& other) const
{
    vector3 tmp(X() - other.X(), Y() - other.Y(), Z() - other.Z());
    return tmp;
}
istream& operator >> (istream &in, vector3 &v)
{
    float x, y, z;
    cout << "Enter vector from the dimensional space:" << endl;
    cout << "x:";
    in >> x;
    v.setX(x);
    cout << "y:";
    in >> y;
    v.setY(y);
    cout << "z:";
    in >> z;
    v.setZ(z);
    return in;
}
ostream& operator<<(ostream &out, vector3 const &v)
{
    out << "(" << v.X() << ", " << v.Y() << ", " << v.Z() << ")";
    return out;
}
vector3& vector3 :: operator+= (vector3 const& other)
{
    setX(this->X()+other.X());
    setY(this->Y()+other.Y());
    setZ(this->Z()+other.Z());
    return *this;
}
vector3& vector3 :: operator-= (vector3 const& other)
{
    setX(this->X()-other.X());
    setY(this->Y()-other.Y());
    setZ(this->Z()-other.Z());
    return *this;
}
vector3 vector3 :: operator=(const vector3& other)
{
    setX(other.X());
    setY(other.Y());
    setZ(other.Z());
    return *this;
}
float vector3 :: operator*(const vector3& other) const
{
    return ((X() * other.X()) + (Y() * other.Y()) + (Z() * other.Z()));
}
vector3 vector3 :: operator^(const vector3& other) const
{
    vector3 res(Y() * other.Z() - Z() * other.Y(), Z() * other.X() - X() * other.Z(), X() * other.Y() - Y() * other.X());
    return res;
}
vector3 vector3 :: operator*(const float a)
{
    setX(X()*a);
    setY(Y()*a);
    setZ(Z()*a);
    return *this;
}
vector3 operator*(const float k, vector3 const& v)
{
    vector3 res(v.X() * k, v.Y() * k, v.Z() * k);
    return res;
}
vector3 vector3 :: operator/(const float a)
{
    if (a != 0)
    {
        vector3 v(X()/a, Y()/a, Z()/a);
        return v;
    }
    else
    {
        cerr << "Error, can`t divide by zero" << endl;
        exit(0);
    }
}

vector3 vector3 :: norm () const
{
    float vLenght = len();
    if(vLenght != 0)
    {
        vector3 res(X()/vLenght, Y()/vLenght, Z()/vLenght);
        return res;
    }
    else
    {
        cerr << "Error, can`t divide by zero" << endl;
        exit(0);
    }
}
vector3 vector3 :: normal(vector3 &v)
{
    vector3 res = (self() ^ v).norm();
    return res;
}
float vector3 :: len() const
{
    float vLenght = sqrt(X() * X() + Y() * Y() + Z() * Z());
    return vLenght;
}
float vector3 :: squareLen()
{
    float sqrLenght = len()*len();
    return sqrLenght;
}
vector3 vector3 :: operator-()
{
    vector3 res = self() * (-1);
    return res;
}
void  vector3 :: rotate (const float degree, const vector3& v)
{
    vector3 e1 = v.norm(), e2 = self(), e3 = (e1 ^ e2).norm(), e4 = (e2 + e3 * tan(degree)).norm();
    float sDegree = acos((e1 * e2)/ e1.len() / e2.len());
    vector3 res = (e1 + e4 * tan (sDegree)).norm() * len();
    setX(round((res.X()*1000))/1000);
    setY(round((res.Y()*1000))/1000);
    setZ(round((res.Z()*1000))/1000);
}
vector3 vector3 :: getRotated (const float degree, const vector3& v)
{
    rotate(degree, v);
    cout << X() << Y() << Z() << endl;
    return *this;
}
//vector3 rotate (const float degree, const vector3&);
