#pragma once

#include <string>
#include <iostream>
#include "../tools/vector.hpp"
#include "../tools/constants.hpp"

using namespace std;


auto runMagnusSimulation(Vect3D initialPosition, Vect3D initialSpeed)
{

    double const rotation(0.001);
    Vect3D const rotationVector(-1, 0, 0);
    double const S0(1);

    double const magnus_const(S0 * rotation / mass);

    Vect3D position(initialPosition);
    Vect3D speed(initialSpeed);
    Vect3D old_speed(speed);
    Vect3D acceleration(0, 0, 0);
    Vect3D old_acceleration(acceleration);

    double const interval(1 / spacing_per_second);
    unsigned int frame(1);

    double coefficient(0), coefficient_magnus(0), speed_size(0);

    vector<tuple<double, double, double>> positions(1, initialPosition.getValues());
    vector<tuple<double, double, double>> speeds(1, initialSpeed.getValues());
    vector<tuple<double, double, double>> accelerations(1, tuple(0, 0, 0));

    while (position.getZ() > 0)
    {
        speed_size = speed.size();
        coefficient = -magnus_const * speed_size;
        coefficient_magnus = magnus_const * speed_size;
        Vect3D magnusForce = vectorialProduct(speed, rotationVector, coefficient_magnus);
        cout << speed_size << " " << coefficient << " " << coefficient_magnus << " "
            << magnusForce.getX() << " " << magnusForce.getY() << " " << magnusForce.getZ() << " b " << gravity_and_archimede_const << endl << endl;
        acceleration.setValue(coefficient * speed.getX() + magnusForce.getX(),
            coefficient * speed.getY() + magnusForce.getY(),
            coefficient * speed.getZ() + gravity_and_archimede_const + magnusForce.getZ());
        speed.addValue(old_acceleration.getX() * interval,
            old_acceleration.getY() * interval,
            old_acceleration.getZ() * interval);
        position.addValue(old_speed.getX() * interval,
            old_speed.getY() * interval,
            old_speed.getZ() * interval);

        old_speed = speed;
        old_acceleration = acceleration;

        frame++;

        positions.push_back(position.getValues());
        speeds.push_back(speed.getValues());
        accelerations.push_back(acceleration.getValues());

        cout << "Time = " << frame / spacing_per_second << "s" << endl
            << "Position:" << endl
            << position.toString() << endl
            << "Speed:" << endl
            << speed.toString() << endl
            << "Acceleration:" << endl
            << acceleration.toString() << endl;

    }    

    return tuple(positions, speeds, accelerations);
}
