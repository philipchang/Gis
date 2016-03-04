#ifndef GPS_OFFSET_2013_H
#define GPS_OFFSET_2013_H

bool outOfChina(double lat, double lon);
double transformLat(double x, double y);
double transformLon(double x, double y);
void gps_transform( double wgLat, double wgLon, double& mgLat, double& mgLon);
#endif
