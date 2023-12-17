#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "../tools/vector.hpp"
#include "../tools/constants.hpp"

using namespace std;

double const rotation_speed(0.001);
Vect3D const rotationVector(-1, 0, 0);
double const S0(1);
double const Rd(0.6);

double const interval(1 / spacing_per_second);
double const magnus_const(S0 * rotation_speed / mass);

Vect3D next_acceleration(Vect3D speed) {
    double speed_norme = speed.size();
    double coefficient_magnus = magnus_const * speed_norme;

    Vect3D dragging_force = -friction_constant * speed_norme * speed;
    Vect3D magnusForce = vectorialProduct(rotationVector, speed, coefficient_magnus);
    return dragging_force + magnusForce + Vect3D(0, 0, gravity_and_archimede_const);
}


auto runRK4simulation(Vect3D initialPosition, Vect3D initialSpeed)
{

    double const magnus_const(S0 * rotation_speed / mass);

    Vect3D position(initialPosition);
    Vect3D speed(initialSpeed);
    Vect3D old_speed(speed);
    Vect3D acceleration(0, 0, 0);

    vector<tuple<double, double, double>> positions(1, initialPosition.getValues());
    vector<tuple<double, double, double>> speeds(1, initialSpeed.getValues());
    vector<tuple<double, double, double>> accelerations(1, tuple(0, 0, 0));

    int frame = 0;

    while (position.getX() >= -2 and position.getX() <= 2
        and position.getY() >= -2 and position.getY() <= 2
        and position.getZ() >= -1 and position.getZ() <= 10
        and frame < 1000){

        if (position.getZ() - minimumDelta <= 0) {

            speed = Vect3D(old_speed.getX()*Rd, old_speed.getY()*Rd, -old_speed.getZ()*Rd);
            acceleration = (speed - old_speed) / interval;
            position = position + interval * speed;

        }
        else {

            Vect3D k1 = next_acceleration(old_speed);
            Vect3D k2 = next_acceleration(old_speed + (interval / 2) * k1);
            Vect3D k3 = next_acceleration(old_speed + (interval / 2) * k2);
            Vect3D k4 = next_acceleration(old_speed + interval * k3);
            speed = old_speed + interval * (k1 + k2 + k3 + k4) / 6;
            acceleration = (speed - old_speed) / interval;
            position = position + interval * speed;

        }

        old_speed = speed;

        positions.push_back(position.getValues());
        speeds.push_back(speed.getValues());
        accelerations.push_back(acceleration.getValues());

        frame++;

    }

    return tuple(positions, speeds, accelerations);

}
