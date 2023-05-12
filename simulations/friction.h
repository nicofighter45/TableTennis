#pragma once

#include <string>
#include <iostream>
#include "../tools/vector.hpp"
#include "../tools/constants.hpp"
#include <vector>

using namespace std;

vector<Vect3D> runFrictionSimulation(Vect3D initialPosition, Vect3D initialSpeed)
{

    double const e((rho * Cx * surface) / (2 * mass));

    Vect3D position(initialPosition);
    Vect3D speed(initialSpeed);
    Vect3D old_speed(speed);
    Vect3D acceleration(0, 0, 0);
    Vect3D old_acceleration(acceleration);

    double const interval(1 / spacing_per_second);
    unsigned int frame(1);

    double coefficient(0), speed_size(0);

    vector<Vect3D> positions(1, position);

    while (position.getZ() > 0)
    {
        speed_size = speed.size();
        coefficient = -e * speed_size;
        acceleration.setValue(coefficient * speed.getX(), coefficient * speed.getY(), coefficient * speed.getZ() - gravity);
        speed.addValue(old_acceleration.getX() * interval, old_acceleration.getY() * interval, old_acceleration.getZ() * interval);
        position.addValue(old_speed.getX() * interval, old_speed.getY() * interval, old_speed.getZ() * interval);

        positions.push_back(position);
        old_speed = speed;
        old_acceleration = acceleration;

        frame++;
    }

    cout << "Time = " << frame / spacing_per_second << "s" << endl
         << "Position:" << endl
         << position.toString() << endl
         << "Speed:" << endl
         << speed.toString() << endl
         << "Acceleration:" << endl
         << acceleration.toString() << endl;

    return positions;
}

Vect3D getFinalStateFriction(Vect3D initialPosition, Vect3D initialSpeed)
{

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

    return position;
}
