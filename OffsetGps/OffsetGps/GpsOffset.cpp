#include "GpsOffset.h"
#include <math.h>


const double pi = 3.14159265358979324;
const double a = 6378245.0;
const double ee = 0.00669342162296594323;
bool outOfChina(double lat, double lon)
{
    if (lon < 72.004 || lon > 137.8347)
        return true;
    if (lat < 0.8293 || lat > 55.8271)
        return true;
    return false;
}


double transformLat(double x, double y)
{
    double ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y + 0.2 * sqrt(abs(x));
    ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
    ret += (20.0 * sin(y * pi) + 40.0 * sin(y / 3.0 * pi)) * 2.0 / 3.0;
    ret += (160.0 * sin(y / 12.0 * pi) + 320 * sin(y * pi / 30.0)) * 2.0 / 3.0;
    return ret;
}


double transformLon(double x, double y)
{
    double ret = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y + 0.1 * sqrt(abs(x));
    ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
    ret += (20.0 * sin(x * pi) + 40.0 * sin(x / 3.0 * pi)) * 2.0 / 3.0;
    ret += (150.0 * sin(x / 12.0 * pi) + 300.0 * sin(x / 30.0 * pi)) * 2.0 / 3.0;
    return ret;
}


void gps_transform( double wgLat, double wgLon, double& mgLat, double& mgLon)
{
	if (outOfChina(wgLat, wgLon))
	{
		mgLat = wgLat;
		mgLon = wgLon;
		return;
    }
	double dLat = transformLat(wgLon - 105.0, wgLat - 35.0);
	double dLon = transformLon(wgLon - 105.0, wgLat - 35.0);
	double radLat = wgLat / 180.0 * pi;
	double magic = sin(radLat);
	magic = 1 - ee * magic * magic;
	double sqrtMagic = sqrt(magic);
	dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi);
	dLon = (dLon * 180.0) / (a / sqrtMagic * cos(radLat) * pi);
	mgLat = wgLat + dLat;
	mgLon = wgLon + dLon;
}
