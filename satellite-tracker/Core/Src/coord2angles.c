#include <coord2angles.h>

// Apply atmospheric refraction to adjust the elevation angle using a standard refraction model
double applyAtmosphericRefraction(double elevation) {
    if (elevation > -1.0 && elevation < 90.0) {
        double tan_elevation = tan((elevation + 7.31 / (elevation + 4.4)) * M_PI / 180.0);
        double refraction = 1.0 / tan_elevation;
        return elevation + refraction / 60.0; // Convert arcminutes to degrees
    }
    return elevation;
}

// Converts spherical to Cartesian (ECEF) coordinates
void spherToCart(double latitude, double longitude, double altitude, double* x, double* y, double* z) {
    const double a = 6378137.0;          // Equatorial radius in meters
    const double f = 1 / 298.257223563;  // Flattening
    const double b = a * (1 - f);        // Polar radius
    const double e2 = 1 - (b * b) / (a * a); // Square of eccentricity

    double latRad = latitude * M_PI / 180.0;
    double lonRad = longitude * M_PI / 180.0;

    double N = a / sqrt(1 - e2 * sin(latRad) * sin(latRad));
    double h = altitude;

    *x = (N + h) * cos(latRad) * cos(lonRad);
    *y = (N + h) * cos(latRad) * sin(lonRad);
    *z = ((b * b) / (a * a) * N + h) * sin(latRad);
}

// Convert ECEF coordinates to ENU coordinates
void ecefToEnu(double satX, double satY, double satZ, double robotX, double robotY, double robotZ, 
               double robotLat, double robotLon, double* east, double* north, double* up) {
    double rbLatRad = robotLat * M_PI / 180.0;
    double rbLonRad = robotLon * M_PI / 180.0;

    double dx = satX - robotX;
    double dy = satY - robotY;
    double dz = satZ - robotZ;

    *east = -sin(rbLonRad) * dx + cos(rbLonRad) * dy;
    *north = -sin(rbLatRad) * cos(rbLonRad) * dx - sin(rbLatRad) * sin(rbLonRad) * dy + cos(rbLatRad) * dz;
    *up = cos(rbLatRad) * cos(rbLonRad) * dx + cos(rbLatRad) * sin(rbLonRad) * dy + sin(rbLatRad) * dz;
}

// Find azimuth angle
double findAzimuth(double satLat, double satLon, double satAlt, double robotLat, double robotLon, double robotAlt) {
    double satX, satY, satZ, robotX, robotY, robotZ;
    spherToCart(satLat, satLon, satAlt, &satX, &satY, &satZ);
    spherToCart(robotLat, robotLon, robotAlt, &robotX, &robotY, &robotZ);

    double east, north, up;
    ecefToEnu(satX, satY, satZ, robotX, robotY, robotZ, robotLat, robotLon, &east, &north, &up);

    double azimuth = atan2(east, north) * (180.0 / M_PI);
    // if (azimuth < 0) azimuth += 360.0;
    return azimuth;
}

// Find elevation angle
double findElevation(double satLat, double satLon, double satAlt, double robotLat, double robotLon, double robotAlt) {
    double satX, satY, satZ, robotX, robotY, robotZ;
    spherToCart(satLat, satLon, satAlt, &satX, &satY, &satZ);
    spherToCart(robotLat, robotLon, robotAlt, &robotX, &robotY, &robotZ);

    double east, north, up;
    ecefToEnu(satX, satY, satZ, robotX, robotY, robotZ, robotLat, robotLon, &east, &north, &up);

    double horizontalDistance = sqrt(east * east + north * north);
    double elevation = atan2(up, horizontalDistance) * (180.0 / M_PI);
    return applyAtmosphericRefraction(elevation);
}
