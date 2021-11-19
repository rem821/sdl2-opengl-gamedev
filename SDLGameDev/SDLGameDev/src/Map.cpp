#pragma once

#include "Map.h"

Map::Map(SDL_Renderer* renderer, bool textureSheet) {
	this->renderer = renderer;
	this->isoEngine = new IsoEngine();

	if(textureSheet) {
		TextureManager::initTextureT(&tilesTexSheet, NULL, NULL, SDL_FLIP_NONE);
		TextureManager::loadTextureT(renderer, &tilesTexSheet, "assets/iso/texturesheet.png");

		int x = 0, y = 0;
		int i;

		for(int i = 0; i < NUM_ISOMETRIC_TILES; ++i) {
			TextureManager::createRect(&tilesRect[i], x, y, TILE_INPUT_SIZE, TILE_INPUT_SIZE);
			x += TILE_INPUT_SIZE;
		}
	} else {
		for(int tile = 0; tile < NUM_ISOMETRIC_TILES; ++tile) {
			TextureManager::initTextureT(&tilesTex[tile], NULL, NULL, SDL_FLIP_NONE);
			TextureManager::loadTextureT(renderer, &tilesTex[tile], fmt::format("assets/iso2/blocks_{}.png", tile + 36).c_str());
		}
	}

	for(int x = 0; x < MAP_WIDTH; ++x) {
		for(int y = 0; y < MAP_HEIGHT; ++y) {
			tiles[x][y].pos = new Point2DT();

			tiles[x][y].texture = &tilesTex[level[x][y]];
		}
	}

}

Map::~Map() {}

void Map::updateIsoMap(Point2DT cameraPos, int scrollAmount) {
	//fmt::print("Camera pos: {} {}\n", cameraPos.x, cameraPos.y);
	TILE_OUTPUT_SIZE = 256 + (5 * scrollAmount);
	for(int x = 0; x < MAP_WIDTH; ++x) {
		for(int y = 0; y < MAP_HEIGHT; ++y) {
			tiles[x][y].pos->x = x * TILE_OUTPUT_SIZE;
			tiles[x][y].pos->y = y * TILE_OUTPUT_SIZE;

			isoEngine->convert2DToIso(tiles[x][y].pos);

			tiles[x][y].pos->x -= cameraPos.x;
			tiles[x][y].pos->y -= cameraPos.y;
		}
	}
}

void Map::drawIsoMap() {
	if(textureSheet) {
		TextureManager::textureRenderXYClip(renderer, &tilesTexSheet, 0, 0, &tilesRect[0]);
	} else {
		for(int x = 0; x < MAP_WIDTH; ++x) {
			for(int y = 0; y < MAP_HEIGHT; ++y) {
				SDL_Rect srcRect;
				srcRect.x = 0;
				srcRect.y = 0;
				srcRect.w = TILE_INPUT_SIZE;
				srcRect.h = TILE_INPUT_SIZE;

				SDL_Rect destRect;
				destRect.x = tiles[x][y].pos->x;
				destRect.y = tiles[x][y].pos->y;
				destRect.w = TILE_OUTPUT_SIZE;
				destRect.h = TILE_OUTPUT_SIZE;

				TextureManager::draw(renderer, tiles[x][y].texture->texture, srcRect, destRect);
			}
		}
	}
}

void Map::drawCursor(SDL_Rect mouseRect) {
	if(textureSheet) {
		TextureManager::textureRenderXYClip(renderer, &tilesTexSheet, mouseRect.x, mouseRect.y, &tilesRect[0]);
	} else {
		SDL_Rect srcRect;
		srcRect.x = TILE_INPUT_SIZE;
		srcRect.y = TILE_INPUT_SIZE;
		TextureManager::draw(renderer, tilesTex[0].texture, srcRect, mouseRect);
	}
}

void Map::drawIsoCursor(SDL_Rect mouseRect, Point2DT cameraPos) {
	mouseRect.x -= TILE_OUTPUT_SIZE * 0.5;
	mouseRect.y -= TILE_OUTPUT_SIZE * 0.5;
	
	mouseRect.x = (mouseRect.x / (TILE_OUTPUT_SIZE / 2)) * TILE_OUTPUT_SIZE / 2;
	mouseRect.y = (mouseRect.y / (TILE_OUTPUT_SIZE / 2)) * TILE_OUTPUT_SIZE / 2;


	if((mouseRect.x % TILE_OUTPUT_SIZE) != 0) {
		mouseRect.y += TILE_OUTPUT_SIZE / 4;
	}

	int correctionX = (cameraPos.x + TILE_OUTPUT_SIZE) % TILE_OUTPUT_SIZE;
	int correctionY = (cameraPos.y + TILE_OUTPUT_SIZE) % TILE_OUTPUT_SIZE;

	mouseRect.x -= correctionX;
	mouseRect.y -= correctionY;

	fmt::print("Correction: {} {}\n", correctionX, correctionY);


	if(textureSheet) {
		TextureManager::textureRenderXYClip(renderer, &tilesTexSheet, mouseRect.x, mouseRect.y, &tilesRect[0]);
	} else {
		SDL_Rect srcRect;
		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = TILE_OUTPUT_SIZE;
		srcRect.h = TILE_OUTPUT_SIZE;

		mouseRect.w = TILE_OUTPUT_SIZE;
		mouseRect.h = TILE_OUTPUT_SIZE;

		TextureManager::draw(renderer, tilesTex[0].texture, srcRect, mouseRect);
	}
}
