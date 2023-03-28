#pragma once

#include <string>
#include <iostream>
#include "../tools/vector.h"
#include "../tools/constants.h"

using namespace std;

void runFrictionSimulation()
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

    double const e((rho * Cx * surface) / (2 * mass));

    Vect3D position(initialPosition);
    Vect3D speed(initialSpeed);
    Vect3D old_speed(speed);
    Vect3D acceleration(0, 0, 0);
    Vect3D old_acceleration(acceleration);

    double const interval(1 / spacing_per_second);
    unsigned int frame(1);

    double coefficient(0), speed_size(0);

    while (position.getZ() > 0)
    {
        speed_size = speed.size();
        coefficient = -e * speed_size;
        acceleration.setValue(coefficient * speed.getX(), coefficient * speed.getY(), coefficient * speed.getZ() - gravity);
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
         << "Acceleration:" << endl 
         << acceleration.getValue() << endl;
         
}
