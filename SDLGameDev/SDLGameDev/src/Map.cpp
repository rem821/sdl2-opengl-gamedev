#pragma once

#include "Map.h"

Map::Map(SDL_Renderer* renderer, bool textureSheet) {
	this->renderer = renderer;

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
			TextureManager::loadTextureT(renderer, &tilesTex[tile], fmt::format("assets/iso2/blocks_{}.png", tile + 0).c_str());
		}
	}

	for(int x = 0; x < MAP_WIDTH; ++x) {
		for(int y = 0; y < MAP_HEIGHT; ++y) {
			tiles[x][y].pos = new Point2DT();
		}
	}

}

Map::~Map() {}

void Map::updateIsoMap(Point2DT cameraPos, int scrollAmount) {
	TILE_OUTPUT_SIZE = 256 + (5 * scrollAmount);
	for(int y = 0; y < MAP_HEIGHT; ++y) {
		for(int x = 0; x < MAP_WIDTH; ++x) {
			tiles[y][x].pos->x = x * TILE_OUTPUT_SIZE;
			tiles[y][x].pos->y = y * TILE_OUTPUT_SIZE;

			IsoEngine::convert2DToIso(tiles[y][x].pos);

			tiles[y][x].pos->x -= cameraPos.x;
			tiles[y][x].pos->y -= cameraPos.y;

			tiles[y][x].texture = &tilesTex[level[y][x] + 36];
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
	Point2DT mousePos, selectedTile;
	mousePos.x = mouseRect.x;
	mousePos.y = mouseRect.y;

	IsoEngine::getTileCoordinates(mousePos, cameraPos, TILE_OUTPUT_SIZE, &selectedTile);

	fmt::print("selected: {}, {}\n", selectedTile.x, selectedTile.y);
	if(selectedTile.x < 0 || selectedTile.y < 0 || selectedTile.x >= MAP_WIDTH || selectedTile.y >= MAP_HEIGHT) return;

	if(textureSheet) {
		TextureManager::textureRenderXYClip(renderer, &tilesTexSheet, mouseRect.x, mouseRect.y, &tilesRect[0]);
	} else {
		SDL_Rect srcRect;
		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = TILE_INPUT_SIZE;
		srcRect.h = TILE_INPUT_SIZE;

		SDL_Rect destRect;
		destRect.x = tiles[selectedTile.y][selectedTile.x].pos->x;
		destRect.y = tiles[selectedTile.y][selectedTile.x].pos->y;
		destRect.w = TILE_OUTPUT_SIZE;
		destRect.h = TILE_OUTPUT_SIZE;

		TextureManager::draw(renderer, tilesTex[0].texture, srcRect, destRect);
	}
}

void Map::onMapTileClick(SDL_Rect mouseRect, Point2DT cameraPos) {
	Point2DT mousePos, selectedTile;
	mousePos.x = mouseRect.x;
	mousePos.y = mouseRect.y;

	IsoEngine::getTileCoordinates(mousePos, cameraPos, TILE_OUTPUT_SIZE, &selectedTile);

	if(selectedTile.x < 0 || selectedTile.y < 0 || selectedTile.x >= MAP_WIDTH || selectedTile.y >= MAP_HEIGHT) return;
	
	level[selectedTile.y][selectedTile.x] += 1;
}

void Map::drawDot(SDL_Rect mouseRect, int type) {
	SDL_Rect srcRect;
	srcRect.w = TILE_INPUT_SIZE;
	srcRect.h = TILE_INPUT_SIZE;
	srcRect.x = 0;
	srcRect.y = 0;

	mouseRect.x -= 10;
	mouseRect.y -= 10;
	mouseRect.w = 20;
	mouseRect.h = 20;

	TextureManager::draw(renderer, tilesTex[type].texture, srcRect, mouseRect);
}
