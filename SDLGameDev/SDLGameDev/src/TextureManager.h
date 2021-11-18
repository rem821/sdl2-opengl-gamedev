#pragma once

#include "CustomTypes.h"
#include "SDL_image.h"
#include "fmt/core.h"

class TextureManager {
	public:
	static SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* filename);
	static void draw(SDL_Renderer* renderer, SDL_Texture* tex, SDL_Rect srcRect, SDL_Rect destRect);

	static void initTextureT(TextureT* tex, SDL_Point* center, SDL_Rect* clipRect, SDL_RendererFlip flipType);
	static void loadTextureT(SDL_Renderer* renderer, TextureT* tex, const char* filename);
	static void textureRenderXYClip(SDL_Renderer* renderer, TextureT* texture, int x, int y, SDL_Rect* clipRect);

	static void createRect(SDL_Rect* rect, int x, int y, int w, int h);
};