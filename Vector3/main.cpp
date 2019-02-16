#include <iostream>
#include "vector3.h"
#include <cmath>

using namespace std;
int main()
{
    vector3 e1(1, 0, 0), e2(0, 1, 0), e3(0, 0, 1), a, b, c;
    float degree = acos((e1 * e2) / e1.len() / e2.len());
    cout << "I/O overload:" << endl;
    cin >> a;
    cout << a << endl;
    cout << "success\n" << endl;

    cout << "operator +, overload:";
    cin >> b;
    cout << "first + second = " << a + b << ", first - second = " << a - b << endl;
    cout << "success\n" << endl;

    cout << "operator +=, -= overload:" << endl;
    a += b;
    cout << "first += second: first = " << a << endl;
    cin >> c;
    b -= c;
    cout << "second -= third: second = " << b << endl;
    cout << "success\n" << endl;

    cout << "operator *, ^ overload:" << endl;
    cout << "b * c = " << b * c << endl;
    cout << "b ^ c = " << (b ^ c) << endl;
    cout << "success\n" << endl;

    cout << "multiplying by a number on both sides:" << endl;
    cout << "5 * c = " << "5 * " << c << " = " << 5 * c << endl;
    cout << "c * 5 = " << c << " * 5" << " = " << 5 * c << endl;
    cout << "success\n" << endl;

    cout << "division by a scalar:" << endl;
    cout << "c / 5 = " << c << " / 5 = " << c / 5 << endl;
    cout << "success\n" << endl;

    cout << "function norm(), len(), and squareLen() = len() * len():" << endl;
    cout << "c.norm() = " << "c / c.len() = " << c << " / " << c.len() << " = " << c.norm() << endl;
    cout << "success\n" << endl;

    cout << "function normal(), rotate(), getRotated()" << endl;
    cout << "e1.normal(e2) = " << e1 << ".normal(" << e2 << ") = " << e1.normal(e2) << endl;
    e1.rotate(-degree, e2);
    cout << "e1.rotate(degree, e2): e1 = " << e1 << endl;
    cout << "e2.getRotated(degree, e3) = " << e2 << ".getRotated(" << degree << ", " << e3 << ") = ";
    cout << e2.getRotated(degree, e3)<< endl;
    cout << "success" << endl;
    return 0;
}
