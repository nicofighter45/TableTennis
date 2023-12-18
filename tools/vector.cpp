#include "vector.hpp"
#include <string>
#include <cmath>
#include <vector>

using namespace std;

Vect3D::Vect3D()
{
    x = 0;
    y = 0;
    z = 0;
}

Vect3D::Vect3D(double ix, double iy, double iz)
{
    x = ix;
    y = iy;
    z = iz;
}

Vect3D::Vect3D(string input)
{
    vector<string> inputs;
    string current("");
    for (unsigned int i = 0; i < input.length(); i++)
    {
        char c = input[i];
        if (c == ' ')
        {
            inputs.push_back(current);
            current = "";
        }
        else
        {
            current += c;
        }
    }
    inputs.push_back(current);
    if (inputs.size() != 3)
    {
        return;
    }
    x = stod(inputs[0]);
    y = stod(inputs[1]);
    z = stod(inputs[2]);
}

double Vect3D::getX() const
{
    return x;
}

double Vect3D::getY() const
{
    return y;
}

double Vect3D::getZ() const
{
    return z;
}

double Vect3D::size() const
{
    return sqrt(sq(x) + sq(y) + sq(z));
}

string Vect3D::toString() const
{
    return "x: " + to_string(x) + " y: " + to_string(y) + " z: " + to_string(z);
}

tuple<double, double, double> Vect3D::getValues() {
    return forward_as_tuple(x, y, z);
}

double sq(double number) {
    return number * number;
}

Vect3D vectorialProduct(const Vect3D& u, const Vect3D& v) {
    return Vect3D(u.getY() * v.getZ() - u.getZ() * v.getY(),
        v.getX() * u.getZ() - u.getX() * v.getZ(), u.getX() * v.getY() - u.getY() * v.getX());
}