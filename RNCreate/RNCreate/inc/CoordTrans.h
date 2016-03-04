#pragma once

#include <math.h>

/************************************************************************/
/* 谷歌瓦片规则 坐标转换                                                */
/************************************************************************/

#ifndef DEG2RAD
#define DEG2RAD 0.017453293
#endif

#ifndef M_PI
#define M_PI 3.14159265f
#endif

// 经纬度坐标点
typedef struct {
	double lon;     ///< 经度
	double lat;		///< 纬度
} LonLat;

// 屏幕坐标点
typedef struct {
	int X;			///< 横坐标
	int Y;			///< 纵坐标
} PixelXY;

// 墨卡托坐标点
typedef struct {
	double X;
	double Y;
} Point2D;

// 瓦片号
typedef struct {
	int X;
	int Y;
} TileXY;


//Geographic coordinates to Mercator coordinates
inline Point2D convertLonLat2MercXY(LonLat lonlat)
{
	Point2D result;

	result.X = lonlat.lon * DEG2RAD;

	double phi = lonlat.lat * DEG2RAD;
	result.Y = 0.5 * log((1 + sin(phi)) / (1 - sin(phi)));

	return result;
}

//Mercator coordinates to Geographic coordinates
inline LonLat convertMercXY2LonLat(Point2D mercXY)
{
	LonLat result;

	result.lon = mercXY.X / DEG2RAD;

	result.lat = atan(sinh(mercXY.Y)) / DEG2RAD;

	return result;
}

//Mercator coordinates to Pixel Points
inline PixelXY convertMercXY2PixelXY(Point2D mercXY, int zoom)
{
	PixelXY result;

	Point2D ltPoint;
	ltPoint.X = -M_PI;
	ltPoint.Y = M_PI;

	Point2D point;
	point.X = mercXY.X - ltPoint.X;
	point.Y = ltPoint.Y - mercXY.Y;

	double ret = (256 * pow(2.0, (zoom-1))) / M_PI;

	result.X = (int)(point.X * ret);
	result.Y = (int)(point.Y * ret);

	return result;
}

//Pixel Points to Mercator coordinates
inline Point2D convertPixelXY2MercXY(PixelXY pixelXY, int zoom)
{
	Point2D result;

	Point2D ltPoint;
	ltPoint.X = -M_PI;
	ltPoint.Y = M_PI;

	double ret = (256 * pow(2.0, (zoom-1))) / M_PI;

	Point2D point;
	point.X = ((double)pixelXY.X) / ret;
	point.Y = ((double)pixelXY.Y) / ret;

	result.X = point.X + ltPoint.X;
	result.Y = ltPoint.Y - point.Y;

	return result;
}

//Tile ID to Pixel Points
inline PixelXY convertTileID2PixelXY(TileXY tileID) {
	PixelXY result;

	result.X = tileID.X * 256;
	result.Y = tileID.Y * 256;

	return result;
}

//Pixel Points to TileID (no zoom)
inline TileXY convertPixelXY2TileID(PixelXY pixelXY) {
	TileXY result;

	result.X = pixelXY.X / 256;
	result.Y = pixelXY.Y / 256;

	return result;
}