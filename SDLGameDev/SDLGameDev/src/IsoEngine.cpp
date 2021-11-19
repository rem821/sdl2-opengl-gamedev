#include "IsoEngine.h"

void IsoEngine::convert2DToIso(Point2DT* point) {
	int tmpX = (point->x - point->y) / 2;
	int tmpY = (point->x + point->y) / 4;
	point->x = tmpX;
	point->y = tmpY;
}

void IsoEngine::convertIsoTo2D(Point2DT* point) {
	int tmpX = (2 * point->y + point->x) * 0.5;
	int tmpY = (2 * point->y - point->x) * 0.5;
	point->x = tmpX;
	point->y = tmpY;
}

void IsoEngine::getTileCoordinates(Point2DT mousePos, Point2DT screenPos, int tileSize, Point2DT* selectedTile) {
	Point2DT cell;
	cell.x = (mousePos.x + screenPos.x) / tileSize;
	cell.y = (mousePos.y + screenPos.y) / (tileSize / 2);

	Point2DT offset;
	offset.x = (mousePos.x + screenPos.x) % tileSize;
	offset.y = (mousePos.y + screenPos.y) % (tileSize / 2);

	selectedTile->x = cell.y + cell.x;
	selectedTile->y = cell.y - cell.x;

	float slope = 0.5;
	float quadW = tileSize / 2;
	float quadH = tileSize / 4;

	if(offset.x <= quadW) {
		if(offset.y <= quadH) {
			if(quadH - slope * offset.x > offset.y) {
				//fmt::print("Leva Horni Venku\n");
				selectedTile->x -= 1;
			} else {
				//fmt::print("Leva Horni\n");
			}
		} else {
			if(slope * offset.x < (offset.y - quadH)) {
				//fmt::print("Leva Dolni Venku\n");
				selectedTile->y += 1;
			} else {
				//fmt::print("Leva Dolni\n");
			}
		}
	} else {
		if(offset.y <= quadH) {
			if(slope * (offset.x - quadW) > offset.y) {
				//fmt::print("Prava Horni Venku\n");
				selectedTile->y -= 1;
			} else {
				//fmt::print("Prava Horni\n");
			}
		} else {
			if(quadH - (slope * (offset.x - quadW)) < (offset.y - quadH)) {
				//fmt::print("Prava Dolni Venku\n");
				selectedTile->x += 1;
			} else {
				//fmt::print("Prava Dolni\n");
			}
		}
	}

}