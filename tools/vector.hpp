#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <string>
#include <cmath>
#include <iostream>

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

    std::tuple<double, double, double> getValues();

    std::string toString() const;

};

double sq(double number);

Vect3D vectorialProduct(const Vect3D& u, const Vect3D& v, double size);

inline Vect3D operator+(const Vect3D& vect1, const Vect3D& vect2) {
    return Vect3D(vect1.getX() + vect2.getX(), 
                    vect1.getY() + vect2.getY(),
                        vect1.getZ() + vect2.getZ());
}

inline Vect3D operator+=(const Vect3D& vect1, const Vect3D& vect2) {
    return vect1 + vect2;
}

inline Vect3D operator-(const Vect3D& vect1, const Vect3D& vect2) {
    return Vect3D(vect1.getX() - vect2.getX(),
        vect1.getY() - vect2.getY(),
        vect1.getZ() - vect2.getZ());
}

inline Vect3D operator-=(const Vect3D& vect1, const Vect3D& vect2) {
    return vect1 - vect2;
}

inline Vect3D operator*(const double scalar, const Vect3D& vect) {
    return Vect3D(scalar * vect.getX(),
                    scalar * vect.getY(),
                        scalar * vect.getZ());
}

inline Vect3D operator*(const Vect3D& vect, const double scalar) {
    return scalar * vect;
}

inline Vect3D operator*=(const Vect3D& vect, const double scalar) {
    return vect * scalar;
}

inline Vect3D operator/(const Vect3D& vect, const double scalar) {
    if (scalar == 0) {
        std::cerr << "Can't divide a vector by 0" << std::endl;
        return Vect3D(0, 0, 0);
    }
    return Vect3D(vect.getX()/scalar,
                    vect.getY()/scalar,
                        vect.getZ()/scalar);
}

inline Vect3D operator/=(const Vect3D& vect, const double scalar) {
    return vect / scalar;
}

inline std::ostream& operator<<(std::ostream& os, const Vect3D& vect) {
    os << vect.toString();
    return os;
}

#endif