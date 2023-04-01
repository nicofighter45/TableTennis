#pragma once

#include <string>
#include <iostream>
#include <vector.h>
#include <constants.h>

using namespace std;

void runMagnusSimulation(Vect3D initialPosition, Vect3D initialSpeed)
{

    double const rotation(1);
    Vect3D const rotationVector(-1, 0, 0);
    double const S0(1);

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
         << "Acceleration:" << endl 
         << acceleration.getValue() << endl;
         
}
