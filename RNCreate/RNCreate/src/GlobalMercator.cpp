#include "GlobalMercator.h"
#include <math.h>

#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif

// 156543.03392804062 for tileSize 256 pixels
double g_initialResolution = 2 * PI * 6378137 / 256;
        
// 20037508.342789244
double g_originShift = 2 * PI * 6378137 / 2.0;

void CGlobalMercator::LatLonToTile(double lat, double lon, size_t & tx, size_t& ty,size_t zoom )
{
	double px, py;
	double mx, my;
	LatLonToMeters( lat, lon,  mx,  my );

	MetersToPixels( px, py,  mx, my, zoom);

	PixelsToTile( px, py,  tx, ty);
}

void CGlobalMercator::GetEveryTileBunds(size_t Lev)
{
	size_t MaxUnit = 1<<Lev;
	for(size_t i = 0; i <= MaxUnit; ++i)
	{
		double minLat,minLon, maxLat, maxLon;
		TileLatLonBounds(i,0,Lev, minLat,  minLon,  maxLat,  maxLon);
				
		// 获取 x = n deg, 描绘（n,-85)->（n,85)
		/*CMapInfoLineData LineData;
		LineData.m_vtPnts.push_back(minLon);
		LineData.m_vtPnts.push_back(-85);
		LineData.m_vtPnts.push_back(minLon);
		LineData.m_vtPnts.push_back(-85);
					
		// 获取 y = n deg, 描绘（-180, n )->（180, n )
		CGlobalMercator::TileLatLonBounds(0,i,Lev, minLat,  minLon,  maxLat,  maxLon);
		LineData.m_vtPnts.clear();
		LineData.m_vtPnts.push_back(-180);
		LineData.m_vtPnts.push_back( minLat);
		LineData.m_vtPnts.push_back(180);
		LineData.m_vtPnts.push_back( minLat);
		outputTitle( mapFileID, &LineData );*/
	}
}

// "Resolution (meters/pixel) for given zoom level (measured at Equator)"
double CGlobalMercator::Resolution(size_t zoom )
{ 
   // # return (2 * PI * 6378137) / (256 * 2**zoom)
    return g_initialResolution / (1<<zoom);		
}

// "Converts given lat/lon in WGS84 Datum to XY in Spherical Mercator EPSG:900913"
void CGlobalMercator::LatLonToMeters(double lat, double lon, double& mx, double& my )
{
	mx = lon * g_originShift / 180.0;
	my = log( tan((90 + lat) * PI / 360.0 )) / (PI / 180.0);	
	my = my * g_originShift / 180.0;
}

//  "Converts XY point from Spherical Mercator EPSG:900913 to lat/lon in WGS84 Datum"
void CGlobalMercator::MetersToLatLon(double& lat, double& lon, double& mx, double& my )
{
    lon = (mx / g_originShift) * 180.0;
    lat = (my / g_originShift) * 180.0;
    lat = 180 / PI * (2 * atan( exp( lat * PI / 180.0)) - PI / 2.0);
}

//  "Converts pixel coordinates in given zoom level of pyramid to EPSG:900913"
void CGlobalMercator::PixelsToMeters(double px, double py, double& mx, double& my, size_t zoom)
{
	double res = Resolution( zoom );
	mx = px * res - g_originShift;
	my = py * res - g_originShift;
}

void CGlobalMercator::PixelsToLatLon(double px, double py, double& lat, double& lon, size_t zoom)
{
	double mx, my;
	PixelsToMeters(px, py, mx, my, zoom);
	MetersToLatLon(lat, lon, mx, my );
}
 
void CGlobalMercator::LatLonToPixels(double lat, double lon, double& px, double& py, size_t zoom)
{
	double mx,  my;
	LatLonToMeters( lat, lon, mx, my );

	MetersToPixels( px, py, mx, my, zoom);
}

//  "Converts EPSG:900913 to pyramid pixel coordinates in given zoom level"
void CGlobalMercator::MetersToPixels(double& px, double& py, double& mx, double& my, size_t zoom)
{               
    double res = Resolution( zoom );
    px = (mx + g_originShift) / res;
    py = (my + g_originShift) / res;
}
    
// "Returns a tile covering region in given pixel coordinates"
void CGlobalMercator::PixelsToTile( double& px, double& py, size_t& tx, size_t& ty)
{  
    tx = int( ceil( px / float(256) ) - 1 );
    ty = int( ceil( py / float(256) ) - 1 );
}

// "Move the origin of pixel coordinates to top-left corner"
void CGlobalMercator::PixelsToRaster(double& px, double& py, size_t zoom)
{
    long mapSize = 256 << zoom;
    py = mapSize - py;
}
     
// "Returns tile for given mercator coordinates"
void CGlobalMercator::MetersToTile(double& mx, double& my, size_t& tx, size_t& ty, size_t zoom)
{
	double px = 0.0, py = 0.0;
    MetersToPixels(px, py, mx, my, zoom);
    return PixelsToTile( px, py, tx, ty);
}

// "Returns bounds of the given tile in EPSG:900913 coordinates"
void CGlobalMercator::TileBounds( size_t& tx, size_t& ty, size_t zoom, 
	double& minx,  double& miny,  double& maxx,  double& maxy)
{        
	PixelsToMeters( tx*256, ty*256, minx, miny, zoom );
    PixelsToMeters( (tx+1)*256, (ty+1)*256, maxx, maxy, zoom );
}
void CGlobalMercator::TilePixelsBounds( size_t& tx, size_t& ty, size_t zoom, 
		double& minx,  double& miny,  double& maxx,  double& maxy)
{
	 minx = tx * 256;
	 miny = ty * 256;
	 maxx = minx + 256;
	 maxy = miny + 256;
}

// "Returns bounds of the given tile in latutude/longitude using WGS84 datum"
void CGlobalMercator::TileLatLonBounds( size_t tx, size_t ty, size_t zoom,
	double& minLat, double& minLon, double& maxLat, double& maxLon )
{        
	double minx, miny, maxx, maxy;
    TileBounds( tx, ty, zoom, minx, miny, maxx, maxy);
    
	MetersToLatLon(minLat, minLon, minx, miny);
    MetersToLatLon(maxLat, maxLon,maxx, maxy);
}
  
// "Maximal scaledown zoom of the pyramid closest to the pixelSize."
size_t CGlobalMercator::ZoomForPixelSize(double pixelSize )
{    
    for(size_t i = 0; i < 30; ++i)
	{
        if( pixelSize > Resolution(i) )
            return i-1;
	}
	return -1;
}

// "Converts TMS tile coordinates to Google Tile coordinates"
void CGlobalMercator::GoogleTile(size_t tx, size_t& ty, size_t zoom)
{   
	// coordinate origin is moved from bottom-left to top-left corner of the extent
	ty = ((1<<zoom) - 1) - ty;
}
