#include "IsoEngine.h"

IsoEngine::IsoEngine(int tileSizeInPixels) {
	if(tileSizeInPixels < 0) {
		tileSize = 64;
	}
}

IsoEngine::~IsoEngine() {}

void IsoEngine::setMapSize(int width, int height) {
	mapHeight = height;
	mapWidth = width;
}

void IsoEngine::convert2DToIso(Point2DT* point) {
	int tmpX = (point->x - point->y) / 2;
	int tmpY = ((point->x + point->y) *0.5) / 2;
	point->x = tmpX;
	point->y = tmpY;
}

void IsoEngine::getTileCoordinates(Point2DT* point, Point2DT* point2DCoord, int tileSize) {
	//point->x = point2DCoord->x;
	//point->y = point2DCoord->y;

	//point->x = (point->x / tileSize) * tileSize;
	//point->y = (point->y / tileSize) * tileSize;

	//point->x -= tileSize * 0.5 * point2DCoord->x / tileSize;
	//point->y += tileSize * 0.25 * point2DCoord->x / tileSize;

	//if(point2DCoord->y > 0) {
	//	point->x -= tileSize * 0.5 * point2DCoord->y / tileSize;
	//	point->y -= tileSize * 0.75 * point2DCoord->y / tileSize;
	//}
	float tempX = (float)point->x / (float)tileSize;
	float tempY = (float)point->y / (float)tileSize;

	point2DCoord->x = (int)tempX;
	point2DCoord->y = (int)tempY;
}