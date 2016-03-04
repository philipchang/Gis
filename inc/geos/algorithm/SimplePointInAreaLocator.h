/**********************************************************************
 * $Id: SimplePointInAreaLocator.h 1820 2006-09-06 16:54:23Z mloskot $
 *
 * GEOS - Geometry Engine Open Source
 * http://geos.refractions.net
 *
 * Copyright (C) 2005-2006 Refractions Research Inc.
 * Copyright (C) 2001-2002 Vivid Solutions Inc.
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the GNU Lesser General Public Licence as published
 * by the Free Software Foundation. 
 * See the COPYING file for more information.
 *
 **********************************************************************/

#ifndef GEOS_ALGORITHM_SIMPLEPOINTINAREALOCATOR_H
#define GEOS_ALGORITHM_SIMPLEPOINTINAREALOCATOR_H


// Forward declarations
namespace geos {
	namespace geom {
		class Geometry;
		class Coordinate;
		class Polygon;
	}
}

namespace geos {
namespace algorithm { // geos::algorithm

class SimplePointInAreaLocator {

public:

	static int locate(const geom::Coordinate& p,
			const geom::Geometry *geom);

	static bool containsPointInPolygon(const geom::Coordinate& p,
			const geom::Polygon *poly);

private:

	static bool containsPoint(const geom::Coordinate& p,
			const geom::Geometry *geom);
};

} // namespace geos::algorithm
} // namespace geos


#endif // GEOS_ALGORITHM_SIMPLEPOINTINAREALOCATOR_H

/**********************************************************************
 * $Log$
 * Revision 1.1  2006/03/09 16:46:48  strk
 * geos::geom namespace definition, first pass at headers split
 *
 **********************************************************************/

