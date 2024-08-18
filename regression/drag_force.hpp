#include "../simulations/RK4.h"
#include "../tools/constants.hpp"
#include "../tools/transfer.hpp"

double getDragCoefficient() {

	Cbz = 0;
	Cm = 0;
	Cd = 0;
	Cbflat = 0;
	Cbrotation = 0;

	Vect3D initialRotation(0, 0, 0);
	Vect3D initialSpeed(0, 0, 0);

	Vect3D initialPosition(0, 0, 0);
	

	auto vectors = runRK4simulation(initialPosition, initialSpeed, initialRotation);

	txtToAllVectors();

	return 0;

}