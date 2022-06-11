#include "IsoEngine.h"

void IsoEngine::convert2DToIso(Point2DT* point, int zLevel, int tileSize) {
	int tmpX = (point->x - point->y) / 2;
	int tmpY = ((point->x + point->y) / 4) - zLevel * (tileSize / 2);
	point->x = tmpX;
	point->y = tmpY;
}

void IsoEngine::convertIsoTo2D(Point2DT* point) {
	int tmpX = (2 * point->y + point->x) * 0.5;
	int tmpY = (2 * point->y - point->x) * 0.5;
	point->x = tmpX;
	point->y = tmpY;
}