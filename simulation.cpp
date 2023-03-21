#include <iostream>
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
        for (char &c : input)
        {
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

int main()
{
    string iniP, iniS;

    cout << "Enter Position Values:" << endl;
    getline(cin, iniP);

    cout << "Enter Speed Values:" << endl;
    getline(cin, iniS);

    Vect3D initialPosition(iniP);
    Vect3D initialSpeed(iniS);

    cout << endl
         << endl
         << "Solving differential equation" << endl;

    double const rotation(1);
    Vect3D const rotationVector(-1, 0, 0);
    double const S0(1);

    double const rho(1.0);
    double const Cx(0.5);
    double const diametre_in_cm(4.0);
    double const mass_in_gram(2.7);
    double const gravity(9.81);
    double const spacing_per_second(1000);

    double const pi(3.141592653589793);
    double const surface(sq(diametre_in_cm * 0.01) * pi * 2 / 3);
    double const mass(mass_in_gram * 0.001);
    double const e((rho * Cx * surface) / (2 * mass));
    double const f(S0 * rotation / mass);

    Vect3D position(initialPosition);
    Vect3D speed(initialSpeed);
    Vect3D old_speed(speed);
    Vect3D acceleration(0, 0, 0);
    Vect3D old_acceleration(acceleration);

    double const interval(1 / spacing_per_second);
    unsigned int frame(1);

    double coefficient(0), coefficient_magnus(0), speed_size(0);

    while (position.getZ() > 0)
    {
        speed_size = speed.size();
        coefficient = -e * speed_size;
        coefficient_magnus = f * speed_size;
        Vect3D magnusForce = vectorialProduct(speed, rotationVector, coefficient_magnus);
        acceleration.setValue(coefficient * speed.getX() + magnusForce.getX(), coefficient * speed.getY() + magnusForce.getY(),
                              coefficient * speed.getZ() - gravity + magnusForce.getZ());
        speed.addValue(old_acceleration.getX() * interval, old_acceleration.getY() * interval, old_acceleration.getZ() * interval);
        position.addValue(old_speed.getX() * interval, old_speed.getY() * interval, old_speed.getZ() * interval);

        old_speed = speed;
        old_acceleration = acceleration;

        frame++;
    }

    cout << "Time = " << frame / spacing_per_second << "s" << endl
         << "Position:" << endl
         << position.getValue() << endl
         << "Speed:" << endl
         << speed.getValue() << endl
         << "Acceleration:" << acceleration.getValue() << endl;

    return 0;
}
