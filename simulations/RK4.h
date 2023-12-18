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
double const Rdz(0.8);
double const Rdflat(0.9);
double const Rdmagnus(0.2);

double const interval(1 / spacing_per_second);
double const magnus_const(S0 * rotation_speed / mass);

Vect3D next_acceleration(Vect3D speed) {
    Vect3D dragging_force = - friction_constant * speed.size() * speed;
    Vect3D magnusForce = magnus_const * vectorialProduct(rotationVector, speed);
    return dragging_force + magnusForce + Vect3D(0, 0, gravity_and_archimede_const);
}


auto runRK4simulation(Vect3D initialPosition, Vect3D initialSpeed)
{

    double const magnus_const(S0 * rotation_speed / mass);

    Vect3D position(initialPosition);
    Vect3D old_position(initialPosition);
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
        and frame < 10000){

        position = position + interval * old_speed;

        Vect3D k1 = next_acceleration(old_speed);
        Vect3D k2 = next_acceleration(old_speed + (interval / 2) * k1);
        Vect3D k3 = next_acceleration(old_speed + (interval / 2) * k2);
        Vect3D k4 = next_acceleration(old_speed + interval * k3);
        acceleration = (k1 + k2 + k3 + k4) / 6;
        speed = old_speed + interval * acceleration;
        
        if (position.getZ() <= 0) {

            double percentage = (old_position.getZ() + position.getZ()) / (old_position.getZ() - position.getZ());
            double new_X = old_position.getX()
                + (old_position.getX() - position.getX()) * percentage
                + Rdflat * (old_position.getX() - position.getX()) * (1 - percentage);
            double new_Y = old_position.getY()
                + (old_position.getY() - position.getY()) * percentage
                + Rdflat * (old_position.getY() - position.getY()) * (1 - percentage);
            double new_Z = -Rdz * (old_position.getZ() - position.getZ()) * (1 - percentage);

            acceleration = Vect3D(acceleration.getX() * Rdflat,
                acceleration.getY() * Rdflat, -Rdz * acceleration.getZ());
            speed = Vect3D(speed.getX() * Rdflat, speed.getY() * Rdflat, -Rdz * speed.getZ());
            position = Vect3D(new_X, new_Y, new_Z);

        }

        old_speed = speed;
        old_position = position;

        positions.push_back(position.getValues());
        speeds.push_back(speed.getValues());
        accelerations.push_back(acceleration.getValues());

        frame++;

    }

    return tuple(positions, speeds, accelerations);

}
