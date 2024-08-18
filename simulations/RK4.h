#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "../tools/vector.hpp"
#include "../tools/constants.hpp"

using namespace std;

Vect3D normalToPlan = Vect3D(0, 0, 1);
Vect3D gravity_force = Vect3D(0, 0, -gravity);

Vect3D next_acceleration(Vect3D speed, Vect3D rotation) {
    Vect3D dragging_force = - friction_constant * speed.size() * speed;
    Vect3D magnus_force = magnus_constant * vectorialProduct(rotation, speed);
    return dragging_force + magnus_force + gravity_force;
}


auto runRK4simulation(Vect3D initialPosition, Vect3D initialSpeed, Vect3D initialRotation)
{

    Vect3D position(initialPosition);
    Vect3D old_position(initialPosition);

    Vect3D speed(initialSpeed);
    Vect3D old_speed(initialSpeed);

    Vect3D rotation(initialRotation);
    Vect3D old_rotation(initialRotation);

    Vect3D initialAcceleration = next_acceleration(speed, rotation);
    Vect3D acceleration(initialAcceleration);
    Vect3D old_acceleration(initialAcceleration);

    vector<tuple<double, double, double>> positions(1, initialPosition.getValues());
    vector<tuple<double, double, double>> speeds(1, initialSpeed.getValues());
    vector<tuple<double, double, double>> rotations(1, rotation.getValues());
    vector<tuple<double, double, double>> accelerations(1, initialAcceleration.getValues());

    int frame = 0;
    int bounce = 0;

    while (position.getX() >= -1 and position.getX() <= 1
        and position.getY() >= -0.5 and position.getY() <= 3
        and position.getZ() >= -1 and position.getZ() <= 2
        and frame < max_frame){

        position = old_position + interval * old_speed;

        Vect3D k1 = next_acceleration(old_speed, old_rotation);
        Vect3D k2 = next_acceleration(old_speed + (interval / 2) * k1, old_rotation);
        Vect3D k3 = next_acceleration(old_speed + (interval / 2) * k2, old_rotation);
        Vect3D k4 = next_acceleration(old_speed + interval * k3, old_rotation);

        acceleration = (k1 + k2 + k3 + k4) / 6;
        speed = old_speed + interval * old_acceleration;
        
        if (position.getZ() <= radius) {

            if (bounce > max_bounce) {
                break;
            }

            bounce++;

            double time_before = old_position.getZ() - radius - position.getZ() / old_speed.getZ();


            double new_X = old_position.getX()
                + old_speed.getX() * time_before
                + Cbflat * (old_position.getX() - position.getX()) * (interval - time_before);
            double new_Y = old_position.getY()
                + old_speed.getY() * time_before
                + Cbflat * (old_position.getY() - position.getY()) * (interval - time_before);
            double new_Z = radius + Cbz * (old_position.getZ() - position.getZ()) * (interval - time_before);
            position = Vect3D(new_X, new_Y, new_Z);

            Vect3D normalizedRotationAdd = vectorialProduct(speed, normalToPlan).normalize();
            Vect3D speedAdd = vectorialProduct(rotation, normalToPlan);

            speed = Vect3D(speed.getX() * Cbflat + speedAdd.getX() * Cbrotation,
                speed.getY() * Cbflat + speedAdd.getY() * Cbrotation, -Cbz * speed.getZ());

            double to_sqrt = mass * (sq(old_speed.size()) - sq(speed.size())) + Jball * sq(old_rotation.size());
            if (to_sqrt < 0) {
                cout << "to_sqrt is negative :( setting it to 0" << endl;
                to_sqrt = 0;
            }
            rotation = normalizedRotationAdd * sqrt(to_sqrt);
            
        }

        old_acceleration = acceleration;
        old_rotation = rotation;
        old_speed = speed;
        old_position = position;

        positions.push_back(position.getValues());
        speeds.push_back(speed.getValues());
        rotations.push_back(rotation.getValues());
        accelerations.push_back(acceleration.getValues());

        frame++;

    }

    return tuple(positions, speeds, rotations, accelerations);

}
