#include <math.h>
#include <stdio.h>

// Constants
//const double earthRadius = 6371000.0;  // Earth's radius in meters
// double robotAlt = 0;

/*
 Required global varibales in main:
 double robotLat, robotLon, robotAlt;
 double satLat, satLon, satAlt;
 double azimuth, elevation;
 */

double applyAtmosphericRefraction(double elevation);
void spherToCart(double latitude, double longitude, double altitude, double* x, double* y, double* z);
void ecefToEnu(double satX, double satY, double satZ, double robotX, double robotY, double robotZ, double robotLat, double robotLon, double* east, double* north, double* up);
// Main functions
double findAzimuth(double satLat, double satLon, double satAlt, double robotLat, double robotLon, double robotAlt);
double findElevation(double satLat, double satLon, double satAlt, double robotLat, double robotLon, double robotAlt);
