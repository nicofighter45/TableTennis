#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

double const spacing_per_second(1000);
double const max_bounce(10);

double const Cd(0.821);
double const Cm(0.069);
double const Cbz(0.8);
double const Cbflat(0.9);
double const Cbrotation(0.6);

double const rho(1.0);
double const air_volumic_mass(1.184);
double const gravity(9.81);
double const diametre_in_cm(4.0);
double const mass_in_gram(2.7);

double const pi(3.141592653589793);

double const interval(1 / spacing_per_second);
double const mass(mass_in_gram * 0.001);
double const radius(diametre_in_cm * 0.01 / 2);
double const surface(pow(radius,2) * 0.0001 * pi * 8 / 3);
double const volume(pi * pow(radius, 3) * 4 / 3 );
double const Jball(mass * pow(radius, 2) * 2 / 5);

double const friction_constant((Cd * air_volumic_mass * surface) / (2 * mass));
double const magnus_constant(Cm * air_volumic_mass * volume / mass);

#endif