#include <iostream>
#include <iomanip>
#include <cmath>


#define G  0.00000000006674
#define Pi 3.14159265359

// Wyliczenie pó³osi wielkiej orbity
double semi_major_axis(double e, double ra) {
	return ra /(1 + e);
}

// Obliczanie okresu orbitalnego
double orbital_period(double a, double M1, double M2) {
	return 2 * Pi * sqrt( pow(a,3)/(G*(M1 + M2)) );
}

// Anomalia œrenida w radianach
double mean_anomaly(double t, double T) {
	return fmod((2 * Pi/ T) * t, 2* Pi);
}

// Rozwi¹zanie równania keplera
double solve_kepler(double M, double e, double epsilon = 1e-6){
	double E = M;
	double delta = 1.0;

	while (fabs(delta) > epsilon) {
		delta = (E - e * sin(E) - M) / (1 - e * cos(E));

		E = E - delta;
	}
	return E;
}

// Wyliczenie pozycji orbitalnych
void orbital_position(double a, double e, double E, double& x, double& y) {
	x = a * (cos(E) - e);
	y = a * sqrt(1 - e * e) * sin(E);
}
