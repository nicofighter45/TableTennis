#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "../tools/vector.hpp"
#include "../tools/constants.hpp"

using namespace std;

Vect3D const rotationVector(-0.001, 0, 0);

Vect3D next_acceleration(Vect3D speed) {

    Vect3D dragging_force = - friction_constant * speed.size() * speed;
    Vect3D magnus_force = magnus_constant * vectorialProduct(rotationVector, speed);
    Vect3D gravity_force = Vect3D(0, 0, -gravity);
    return dragging_force + magnus_force + gravity_force;

}


auto runRK4simulation(Vect3D initialPosition, Vect3D initialSpeed)
{

    Vect3D position(initialPosition);
    Vect3D old_position(initialPosition);

    Vect3D speed(initialSpeed);
    Vect3D old_speed(speed);

    Vect3D initialAcceleration = next_acceleration(speed);
    Vect3D acceleration(initialAcceleration);
    Vect3D old_acceleration(initialAcceleration);

    vector<tuple<double, double, double>> positions(1, initialPosition.getValues());
    vector<tuple<double, double, double>> speeds(1, initialSpeed.getValues());
    vector<tuple<double, double, double>> accelerations(1, initialAcceleration.getValues());

    int frame = 0;
    int bounce = 0;

    while (position.getX() >= -1 and position.getX() <= 1
        and position.getY() >= -0.5 and position.getY() <= 1.5
        and position.getZ() >= -1 and position.getZ() <= 10
        and frame < 10000){

        position = position + interval * old_speed;

        Vect3D k1 = next_acceleration(old_speed);
        Vect3D k2 = next_acceleration(old_speed + (interval / 2) * k1);
        Vect3D k3 = next_acceleration(old_speed + (interval / 2) * k2);
        Vect3D k4 = next_acceleration(old_speed + interval * k3);

        acceleration = (k1 + k2 + k3 + k4) / 6;
        speed = old_speed + interval * old_acceleration;
        
        if (position.getZ() <= 0) {

            if (bounce > max_bounce) {
                break;
            }

            bounce++;

            double percentage = abs((old_position.getZ() + position.getZ()) / (old_position.getZ() - position.getZ()));
            if (percentage < 0 or percentage > 1) {
                cerr << "Percentage can't be out of 0 to 1 :" << endl
                    << "Old_pos: " << old_position << endl
                    << "Pos: " << position << endl
                    << "Percentage: " << percentage << endl;
                break;
            }

            double new_X = old_position.getX()
                + (old_position.getX() - position.getX()) * percentage
                + Cbflat * (old_position.getX() - position.getX()) * (1 - percentage);
            double new_Y = old_position.getY()
                + (old_position.getY() - position.getY()) * percentage
                + Cbflat * (old_position.getY() - position.getY()) * (1 - percentage);
            double new_Z = -Cbz * (old_position.getZ() - position.getZ()) * (1 - percentage);

            acceleration = Vect3D(acceleration.getX() * Cbflat,
                acceleration.getY() * Cbflat, -Cbz * acceleration.getZ());
            speed = Vect3D(speed.getX() * Cbflat, speed.getY() * Cbflat, -Cbz * speed.getZ());
            position = Vect3D(new_X, new_Y, new_Z);

        }

        old_acceleration = acceleration;
        old_speed = speed;
        old_position = position;

        positions.push_back(position.getValues());
        speeds.push_back(speed.getValues());
        accelerations.push_back(acceleration.getValues());

        frame++;

    }

    return tuple(positions, speeds, accelerations);

}
