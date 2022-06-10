#pragma once

#include "Map.h"

Map::Map(SDL_Renderer* renderer, bool textureSheet) {
	this->renderer = renderer;

	if(textureSheet) {
		TextureManager::initTextureT(&tilesTexSheet, NULL, NULL, SDL_FLIP_NONE);
		TextureManager::loadTextureT(renderer, &tilesTexSheet, "assets/iso/texturesheet.png");

		int x = 0, y = 0;

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

	for(int z = 0; z < MAP_DEPTH; ++z) {
		loadLevel(z);
		for(int x = 0; x < MAP_HEIGHT; ++x) {
			for(int y = 0; y < MAP_WIDTH; ++y) {
				tiles[z][y][x].pos = new Point2DT();
				fmt::print(fmt::format("{},{},{}\n", z, y, x));
			}
		}
	}

}

Map::~Map() {}

void Map::updateIsoMap(SDL_Rect mouseRect, Point2DT cameraPos, int scrollAmount) {
	TILE_OUTPUT_SIZE = 256 + (5 * scrollAmount);
	for(int y = 0; y < MAP_HEIGHT; ++y) {
		for(int x = 0; x < MAP_WIDTH; ++x) {
			tiles[0][y][x].pos->x = x * TILE_OUTPUT_SIZE;
			tiles[0][y][x].pos->y = y * TILE_OUTPUT_SIZE;

			IsoEngine::convert2DToIso(tiles[0][y][x].pos);

			tiles[0][y][x].texture = &tilesTex[level[0][y][x] + 36];

			Point2DT worldPos;
			worldPos.x = mouseRect.x + cameraPos.x;
			worldPos.y = mouseRect.y + cameraPos.y;
			int startX = tiles[0][y][x].pos->x;
			int startY = tiles[0][y][x].pos->y;
			int endX = startX + TILE_OUTPUT_SIZE;
			int endY = startY + TILE_OUTPUT_SIZE / 2;

			if(worldPos.x > startX && worldPos.x < endX && worldPos.y > startY && worldPos.y < endY) {

				highlightedTile.x = x;
				highlightedTile.y = y;

				refineTilePosition(&worldPos);
			}
		}
	}
}

void Map::drawIsoMap(Point2DT cameraPos) {
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
				destRect.x = tiles[0][y][x].pos->x - cameraPos.x;
				destRect.y = tiles[0][y][x].pos->y - cameraPos.y;
				destRect.w = TILE_OUTPUT_SIZE;
				destRect.h = TILE_OUTPUT_SIZE;

				TextureManager::draw(renderer, tiles[0][y][x].texture->texture, srcRect, destRect);
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
	Point2DT worldPos;

	if(highlightedTile.x < 0 || highlightedTile.y < 0 || highlightedTile.x >= MAP_WIDTH || highlightedTile.y >= MAP_HEIGHT) return;

	if(textureSheet) {
		TextureManager::textureRenderXYClip(renderer, &tilesTexSheet, mouseRect.x, mouseRect.y, &tilesRect[0]);
	} else {
		SDL_Rect srcRect;
		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = TILE_INPUT_SIZE;
		srcRect.h = TILE_INPUT_SIZE;

		SDL_Rect destRect;
		destRect.x = tiles[0][highlightedTile.y][highlightedTile.x].pos->x - cameraPos.x;
		destRect.y = tiles[0][highlightedTile.y][highlightedTile.x].pos->y - cameraPos.y;
		destRect.w = TILE_OUTPUT_SIZE;
		destRect.h = TILE_OUTPUT_SIZE;

		TextureManager::draw(renderer, tilesTex[0].texture, srcRect, destRect);
		//fmt::print("highlighting new tile: {} {}\n", highlightedTile.x, highlightedTile.y);
	}
}

void Map::onMapTileClick() {

	if(highlightedTile.x < 0 || highlightedTile.y < 0 || highlightedTile.x >= MAP_WIDTH || highlightedTile.y >= MAP_HEIGHT) return;

	level[0][highlightedTile.y][highlightedTile.x] += 1;
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

void Map::refineTilePosition(Point2DT* worldPos) {
	float slope = 0.5;
	float quadW = TILE_OUTPUT_SIZE / 2;
	float quadH = TILE_OUTPUT_SIZE / 4;

	Point2DT offset;
	offset.x = highlightedTile.x - worldPos->x;
	offset.y = highlightedTile.y - worldPos->y;

	if(offset.x <= quadW) {
		if(offset.y <= quadH) {
			if(quadH - slope * offset.x > offset.y) {
				//fmt::print("Leva Horni Venku\n");
				highlightedTile.x -= 1;
			} else {
				//fmt::print("Leva Horni\n");
			}
		} else {
			if(slope * offset.x < (offset.y - quadH)) {
				//fmt::print("Leva Dolni Venku\n");
				highlightedTile.y += 1;
			} else {
				//fmt::print("Leva Dolni\n");
			}
		}
	} else {
		if(offset.y <= quadH) {
			if(slope * (offset.x - quadW) > offset.y) {
				//fmt::print("Prava Horni Venku\n");
				highlightedTile.y -= 1;
			} else {
				//fmt::print("Prava Horni\n");
			}
		} else {
			if(quadH - (slope * (offset.x - quadW)) < (offset.y - quadH)) {
				//fmt::print("Prava Dolni Venku\n");
				highlightedTile.x += 1;
			} else {
				//fmt::print("Prava Dolni\n");
			}
		}
	}
}

void Map::loadLevel(int z) {
	std::ifstream infile(fmt::format("assets/map/level_{}.txt", z));

	int x = 0;
	int y = 0;

	std::string line;
	while(std::getline(infile, line)) {
		std::istringstream iss(line);

		for(int i = 0; i < line.size(); ++i) {
			char ch = line[i];

			if(ch == '1' || ch == '0') {
				level[z][y][x] = ch - '0';
				x += 1;
			}
		}
		y += 1;
		x = 0;
	}

	infile.close();
	//fmt::print(fmt::format("Level {} processed\n", z));
}