#pragma once

#include "CustomTypes.h"

class IsoEngine {
	public:
	static void convert2DToIso(Point2DT* point);
	static void convertIsoTo2D(Point2DT* point);
	static void getTileCoordinates(Point2DT mousePos, Point2DT screenPos, int tileSize, Point2DT* selectedTile);
};

