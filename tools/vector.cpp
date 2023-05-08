#pragma once

#include "vector.h"
#include <string>
#include <cmath>
#include <vector>

using namespace std;

Vect3D vectorialProduct(const Vect3D& u, const Vect3D& v, double size) {
    const double x(u.getY() * v.getZ() - u.getZ() * v.getY()), y(v.getX() * u.getZ() - u.getX() * v.getZ()), z(u.getX() * v.getY() - u.getY() * v.getX());
    size /= sqrt(sq(x) + sq(y) + sq(z));
    return Vect3D(x * size, y * size, z * size);
}

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
    return std::sqrt(sq(x) + sq(y) + sq(z));
}

string Vect3D::toString() const
{
    return "X = " + to_string(x) + "\nY = " + to_string(y) + "\nZ = " + to_string(z);
}

void Vect3D::addValue(double xt = 0, double yt = 0, double zt = 0)
{
    x += xt;
    y += yt;
    z += zt;
}

void Vect3D::setValue(double xt, double yt, double zt)
{
    x = xt;
    y = yt;
    z = zt;
}