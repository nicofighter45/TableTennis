#include <math.h>
#include <stdio.h>

typedef struct Vect3D Vect3D;
struct Vect3D {
    double X;
    double Y;
    double Z;
};

double sq(double number)
{
    return number * number;
}

int main_old() {

    const Vect3D initialPosition = {0, 0, 0.15};
    const Vect3D initialSpeed = {0, 4, 1};
    const double rho = 1.0;
    const double Cx = 0.5;
    const double diametre_in_cm = 4.0;
    const double mass_in_gram = 2.7;
    const double gravity = 9.81;
    const double spacing_per_second = 100;

    const double pi = 3.141592653589793;
    const double surface = 2 / 3 * pi * sq(diametre_in_cm * 0.01);
    const double mass = mass_in_gram * 0.001;
    const double e = (rho * Cx * surface) / (2 * mass);


    Vect3D position = initialPosition;
    Vect3D speed = initialSpeed;
    Vect3D old_speed = speed;
    Vect3D acceleration = {0, 0, 0};
    Vect3D old_acceleration = acceleration;

    const double interval = 1 / spacing_per_second;
    unsigned int frame = 1;

    while(position.Z > 0){
        const double coefficient = - e * sqrt(sq(speed.X) + sq(speed.Y) + sq(speed.Z));
        acceleration.X = coefficient * speed.X;
        acceleration.Y = coefficient * speed.Y;
        acceleration.Z = coefficient * speed.Z - gravity;

        speed.X += old_acceleration.X * interval;
        speed.Y += old_acceleration.Y * interval;
        speed.Z += old_acceleration.Z * interval;

        position.X += old_speed.X * interval;
        position.Y += old_speed.Y * interval;
        position.Z += old_speed.Z * interval;

        old_speed = speed;
        old_acceleration = acceleration;

        frame += 1;
    }

    printf("Time = %d\nPosition:  X = %f  Y = %f  Z = %f\n", frame, position.X, position.Y, position.Z);
    printf("Speed:  X = %f  Y = %f  Z = %f\n", speed.X, speed.Y, speed.Z);
    printf("Acceleration:  X = %f  Y = %f  Z = %f\n\n", acceleration.X, acceleration.Y, acceleration.Z);

    return 0;

}