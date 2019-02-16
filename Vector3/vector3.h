#pragma once
#include <iostream>
using namespace std;
class vector3
{
public:
    vector3 self();
    vector3 (float x, float y, float z);
    vector3 ();
    float X() const;
    float Y() const;
    float Z() const;
    void setX(float x);
    void setY(float y);
    void setZ(float z);
    vector3 operator+(vector3 const& other) const;
    vector3 operator-(vector3 const& other) const;
    vector3 operator=(const vector3& other);
    vector3& operator+=(const vector3& other);
    vector3& operator-=(const vector3& other);
    float operator*(const vector3& other) const;
    vector3 operator^(const vector3& other) const;
    vector3 operator*(const float a);
    vector3 operator/(const float a);
    vector3 norm () const;
    vector3 normal(vector3 &v);
    float len() const;
    float squareLen();
    vector3 operator-();
    void rotate (const float degree, const vector3& v);
    vector3 getRotated (const float degree, const vector3& v);
private:
    float x, y, z;
};
istream& operator>>(istream &in, vector3 &v);
ostream& operator<<(ostream &out, vector3 const &v);
vector3 operator*(const float k, vector3 const & v);
