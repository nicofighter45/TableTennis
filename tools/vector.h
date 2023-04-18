#pragma once

#include <string>
#include <cmath>
#include <vector>

using namespace std;

double sq(double number)
{
    return number * number;
}

class Vect3D
{

private:
    double x;
    double y;
    double z;

public:
    double getX() { return x; };
    double getY() { return y; };
    double getZ() { return z; };

    Vect3D(double ix, double iy, double iz)
    {
        x = ix;
        y = iy;
        z = iz;
    }

    Vect3D(string input)
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

    void addValue(double xt = 0, double yt = 0, double zt = 0)
    {
        x += xt;
        y += yt;
        z += zt;
    }

    void setValue(double xt, double yt, double zt)
    {
        x = xt;
        y = yt;
        z = zt;
    }

    double size()
    {
        return sqrt(sq(x) + sq(y) + sq(z));
    }

    string getValue()
    {
        return "X = " + to_string(x) + "\nY = " + to_string(y) + "\nZ = " + to_string(z);
    }
};

Vect3D vectorialProduct(Vect3D u, Vect3D v, double size)
{
    double x(u.getY() * v.getZ() - u.getZ() * v.getY()), y(v.getX() * u.getZ() - u.getX() * v.getZ()), z(u.getX() * v.getY() - u.getY() * v.getX());
    size /= sqrt(sq(x) + sq(y) + sq(z));
    return Vect3D(x * size, y * size, z * size);
}