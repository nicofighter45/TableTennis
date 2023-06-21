#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

double const rho(1.0);
double const Cx(0.5);
double const diametre_in_cm(4.0);
double const mass_in_gram(2.7);
double const gravity(9.81);
double const air_volumic_mass(1.292);
double const spacing_per_second(1000);

double const pi(3.141592653589793);
double const surface(diametre_in_cm * diametre_in_cm * 0.0001 * pi * 2 / 3);
double const mass(mass_in_gram * 0.001);
double const volume(pi * pow((0.01 * diametre_in_cm / 2), 3) * 4 / 3 );

double const gravity_and_archimede_const((volume* air_volumic_mass / mass - 1) * gravity);
double const frottement_const((rho* Cx* surface) / (2 * mass));

#endif