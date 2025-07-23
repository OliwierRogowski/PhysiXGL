#pragma once

#define Pi 3.141592653589793
#define G  0.00000000006674

double semi_major_axis(double e, double ra);

double orbital_period(double a, double M1, double M2);

double mean_anomaly(double t, double T);

double solve_kepler(double M, double e, double epsilon = 1e-6);

void orbital_position(double a, double e, double E, double& x, double& y);
