#pragma once

#include <iostream>
#include <vector.h>
#include <constants.h>
#include <cmath>

using namespace std;
;

void runGravitySimulation(Vect3D initialPosition, Vect3D initialSpeed)
{
    double const time = (sqrt(sq(initialSpeed.getZ()) + 2 * gravity * initialPosition.getZ())-initialSpeed.getZ())/2;
    Vect3D position(initialPosition), speed(initialSpeed), acceleration(0, 0, -9.81);
    cout << "Time = " << time << "s" << endl
         << "Position:" << endl
         << position.getValue() << endl
         << "Speed:" << endl
         << speed.getValue() << endl
         << "Acceleration:" << endl
         << acceleration.getValue() << endl;
}
