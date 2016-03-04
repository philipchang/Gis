/***************************************************
 * @File: GlobalMercator.h
 * @Description: mercator project, tile map service operator
 * @Author: changggy 2014-9-30
 * copy all right by AutoPia
 ***************************************************/

#ifndef GLOBAL_MERCATOR_H
#define GLOBAL_MERCATOR_H

#pragma once
#include <math.h>

#ifndef PI
#define PI 3.141592653589
#endif

/************************************************************************/
/* VirtualEarth 投影计算式    
/* 墨卡托投影模式                                                       */
/************************************************************************/
class  CVEProjection
{
public :

	static double l2m(double y1)
	{
		double y2;
		y1 = y1 * PI / 180.0;
		y2 = sin(y1);
		y2 = log((1.0 + y2) / (1.0 - y2)) / 2.0;
		y2 = y2 * 180.0 / PI;

		return y2;
	}

	static double m2l(double y2)
	{
		double y1;
		y2 = y2 * PI / 180.0;
		y1 = (atan(exp(y2)) - PI / 4.0) * 2.0;
		y1 = y1 * 180.0 / PI;
		
		return y1;
	}
};

class CGlobalMercator
{
public:
	CGlobalMercator(){};
	~CGlobalMercator(){};

	static void LatLonToTile(double lat, double lon, size_t & x, size_t& y,size_t zoom );

	static void GetEveryTileBunds(size_t Lev);

	// "Resolution (meters/pixel) for given zoom level (measured at Equator)"
	static double Resolution(size_t zoom );

	// "Converts given lat/lon in WGS84 Datum to XY in Spherical Mercator EPSG:900913"
	static void LatLonToMeters(double lat, double lon, double& mx, double& my );

	//  "Converts XY point from Spherical Mercator EPSG:900913 to lat/lon in WGS84 Datum"
	static void MetersToLatLon(double& lat, double& lon, double& mx, double& my );
	//  "Converts pixel coordinates in given zoom level of pyramid to EPSG:900913"
	static void PixelsToMeters(double px, double py, double& mx, double& my, size_t zoom);

	static void PixelsToLatLon(double px, double py, double& lat, double& lon, size_t zoom);	

	// lat lon 2 pixels
	static void LatLonToPixels(double lat, double lon, double& px, double& py, size_t zoom);

	//  "Converts EPSG:900913 to pyramid pixel coordinates in given zoom level"
	static void MetersToPixels(double& px, double& py, double& mx, double& my, size_t zoom);
    
	// "Returns a tile covering region in given pixel coordinates"
	static void PixelsToTile( double& px, double& py, size_t& tx, size_t& ty);

	// "Move the origin of pixel coordinates to top-left corner"
	static void PixelsToRaster(double& px, double& py, size_t zoom);
     
	// "Returns tile for given mercator coordinates"
	static void MetersToTile(double& mx, double& my, size_t& tx, size_t& ty, size_t zoom);

	// "Returns bounds of the given tile in EPSG:900913 coordinates"
	static void TileBounds( size_t& tx, size_t& ty, size_t zoom, 
		double& minx,  double& miny,  double& maxx,  double& maxy);

	// "Returns bounds of the Pixels"
	static void TilePixelsBounds( size_t& tx, size_t& ty, size_t zoom, 
		double& minx,  double& miny,  double& maxx,  double& maxy);

	// "Returns bounds of the given tile in latutude/longitude using WGS84 datum"
	static void TileLatLonBounds( size_t tx, size_t ty, size_t zoom,
		double& minLat, double& minLon, double& maxLat, double& maxLon );
  
	// "Maximal scaledown zoom of the pyramid closest to the pixelSize."
	static size_t ZoomForPixelSize(double pixelSize );

	// "Converts TMS tile coordinates to Google Tile coordinates"
	static void GoogleTile(size_t tx, size_t& ty, size_t zoom);
};


#endif