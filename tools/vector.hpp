#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <string>
#include <cmath>

class Vect3D
{

private:
    double x;
    double y;
    double z;

public:

    Vect3D();

    Vect3D(double ix, double iy, double iz);

    Vect3D(std::string input);

    double getX() const;
    double getY() const;
    double getZ() const;

    double size() const;

    void addValue(double xt = 0, double yt = 0, double zt = 0);

    void setValue(double xt, double yt, double zt);

    std::tuple<double, double, double> getValues();

    std::string toString() const;

};

double sq(double number);

Vect3D vectorialProduct(const Vect3D& u, const Vect3D& v, double size);

#endif