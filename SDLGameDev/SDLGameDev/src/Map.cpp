#pragma once

#include "Map.h"
#include "TextureManager.h"

int level[20][25] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{2,2,2,2,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{2,2,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{2,2,2,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{2,2,2,2,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{2,2,2,2,2,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,2,2,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,2,2,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,2,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,2,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,2,2,2,2,2,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,2,2,2,2,2,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,2,2,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

Map::Map() {
	dirt = TextureManager::loadTexture("assets/dirt.png");
	grass = TextureManager::loadTexture("assets/grass.png");
	water = TextureManager::loadTexture("assets/water.png");

	loadMap(level);

	srcRect.x = srcRect.y = 0;
	srcRect.w = destRect.w = 64;
	srcRect.h = destRect.h = 64;

	destRect.x = destRect.y = 0;
}

Map::~Map() {
}

void Map::loadMap(int arr[20][25]) {
	for(int row = 0; row < 20; row++) {
		for(int column = 0; column < 25; column++) {
			map[row][column] = arr[row][column];
		}
	}

}

void Map::drawMap() {
	int type = 0;

	for(int row = 0; row < 20; row++)
		for(int column = 0; column < 25; column++) {
			type = map[row][column];

			destRect.x = column * 64;
			destRect.y = row * 64;

			switch(type) {
			case 0:
				TextureManager::draw(dirt, srcRect, destRect);
				break;
			case 1:
				TextureManager::draw(grass, srcRect, destRect);
				break;
			case 2:
				TextureManager::draw(water, srcRect, destRect);
				break;
			default:
				break;
			}
		}
}

