#pragma once

#include "SDL.h"

typedef struct Point2DT {
	int x;
	int y;
}Point2DT;

typedef struct TextureT {
	SDL_Point* center;
	SDL_Rect* clipRect;
	SDL_RendererFlip flipType;
	SDL_Texture* texture;
}TextureT;

typedef struct TileT {
	Point2DT* pos;
	TextureT* texture;
}TileT;


