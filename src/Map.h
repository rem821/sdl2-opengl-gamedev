#pragma once

#include "CustomTypes.h"
#include "TextureManager.h"
#include "IsoEngine.h"
#include "fmt/core.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define NUM_ISOMETRIC_TILES 101
#define TILE_INPUT_SIZE 64

#define MAP_HEIGHT 10
#define MAP_WIDTH 10
#define MAP_DEPTH 1

class Map {
	public:
	Map(SDL_Renderer* renderer);
	~Map();

	void updateIsoMap(SDL_Rect mouseRect, Point2DT cameraPos, int scrollAmount);
	void drawIsoMap(Point2DT cameraPos);
	void drawCursor(SDL_Rect mouseRect);

	void drawIsoCursor(SDL_Rect mouseRect, Point2DT cameraPos);

	void onMapTileClick();

	void drawDot(SDL_Rect mouseRect, int type);

	private:
	void refineTilePosition(Point2DT* worldPos);

	void loadLevel(int level);

	SDL_Renderer* renderer;

	TextureT tilesTexSheet;
	SDL_Rect tilesRect[NUM_ISOMETRIC_TILES];

	TextureT tilesTex[NUM_ISOMETRIC_TILES];
	TileT tiles[MAP_DEPTH][MAP_WIDTH][MAP_HEIGHT];
	Point2DT highlightedTile;

	int TILE_OUTPUT_SIZE = 256;

	int level[MAP_DEPTH][MAP_WIDTH][MAP_HEIGHT];
};