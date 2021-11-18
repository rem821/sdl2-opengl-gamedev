#pragma once

#include "CustomTypes.h"

class IsoEngine {
	public:
	IsoEngine(int tileSizeInPixels);
	~IsoEngine();

	void setMapSize(int width, int height);

	void convert2DToIso(Point2DT* point);

	void getTileCoordinates(Point2DT* point, Point2DT* point2DCoord, int tileSize);

	inline int getTileSize() { return tileSize; };

	private:
	int tileSize = 64;
	int mapHeight;
	int mapWidth;
};

