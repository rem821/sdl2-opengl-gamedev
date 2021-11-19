#include "IsoEngine.h"

IsoEngine::IsoEngine() {}

IsoEngine::~IsoEngine() {}

void IsoEngine::convert2DToIso(Point2DT* point) {
	int tmpX = (point->x - point->y) / 2;
	int tmpY = ((point->x + point->y) *0.5) / 2;
	point->x = tmpX;
	point->y = tmpY;
}