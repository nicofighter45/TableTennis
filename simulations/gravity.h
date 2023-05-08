#pragma once

#include <iostream>
#include "../tools/vector.h"
#include "../tools/constants.cpp"
#include <cmath>

using namespace std;

static Vect3D getVector(double time, Vect3D initialPosition, Vect3D initialSpeed)
{
    return Vect3D(initialSpeed.getX() * time + initialPosition.getX(),
                  initialSpeed.getY() * time + initialPosition.getY(),
                  -gravity / 2 * sq(time) + initialSpeed.getZ() * time + initialPosition.getZ());
}

vector<Vect3D> runGravitySimulation(Vect3D initialPosition, Vect3D initialSpeed)
{
    vector<Vect3D> positions(1, initialPosition);
    double const final_time = (sqrt(sq(initialSpeed.getZ()) + 2 * gravity * initialPosition.getZ()) - initialSpeed.getZ()) / 2;

    double const interval(1 / spacing_per_second);

    for (double time = interval; time < final_time; time += interval)
    {
        positions.push_back(getVector(time, initialPosition, initialSpeed));
    }

    Vect3D position(initialPosition), speed(initialSpeed), acceleration(0, 0, -9.81);
    cout << "Time = " << final_time << "s" << endl
         << "Position:" << endl
         << position.toString() << endl
         << "Speed:" << endl
         << speed.toString() << endl
         << "Acceleration:" << endl
         << acceleration.toString() << endl;

    return positions;
}

Vect3D getGravitySimulationEnd(Vect3D initialPosition, Vect3D initialSpeed)
{
    double const time = (sqrt(sq(initialSpeed.getZ()) + 2 * gravity * initialPosition.getZ()) - initialSpeed.getZ()) / 2;
    return getVector(time, initialPosition, initialSpeed);
}