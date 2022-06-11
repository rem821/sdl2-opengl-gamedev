#pragma once

#include "CustomTypes.h"
#include "TextureManager.h"
#include "IsoEngine.h"
#include "fmt/core.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>

#define NUM_ISOMETRIC_TILES 102
#define TILE_INPUT_SIZE 64

#define MAP_HEIGHT 20
#define MAP_WIDTH 20
#define MAP_DEPTH 2

class Map {
	public:
	Map(SDL_Renderer* renderer);
	~Map();

	void updateIsoMap(SDL_Rect mouseRect, Point2DT cameraPos, int scrollAmount);
	void drawIsoMap(Point2DT cameraPos, int cameraRot);
	void drawCursor(SDL_Rect mouseRect);


	void onMapTileClick();

	void drawDot(SDL_Rect mouseRect, int type);

	private:
    void drawIsoCursor(Point2DT cameraPos, int cameraRot);

    void highlightIfSelected(Point2DT cursorAbsPos, int x, int y, int z);

	void loadLevel(int level);

	SDL_Renderer* renderer;

	TextureT tilesTexSheet;
	SDL_Rect tilesRect[NUM_ISOMETRIC_TILES];

	TextureT tilesTex[NUM_ISOMETRIC_TILES];
	TileT tiles[MAP_DEPTH][MAP_WIDTH][MAP_HEIGHT];
	Point3DT highlightedTile;

	int TILE_OUTPUT_SIZE = 256;

	int level[MAP_DEPTH][MAP_WIDTH][MAP_HEIGHT];
};