#include "TextureManager.h"

SDL_Texture* TextureManager::loadTexture(SDL_Renderer* renderer, const char* filename) {
    fmt::print("Loading texture {}\n", filename);
    SDL_Surface* tmpSurface = IMG_Load(filename);
	if(tmpSurface == NULL) {
		fmt::print("Texture error: Couldn't load image: {}\n", filename);
		return NULL;
	}

	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	if(tex == NULL) {
		fmt::print("Texture error: Couldn't load image: {}\n", filename);
	}

    fmt::print("Texture loaded {}\n", filename);
    SDL_FreeSurface(tmpSurface);
	return tex;
}

void TextureManager::draw(SDL_Renderer* renderer, SDL_Texture* tex, SDL_Rect srcRect, SDL_Rect destRect) {
	SDL_RenderCopy(renderer, tex, &srcRect, &destRect);
}

void TextureManager::initTextureT(TextureT* tex, SDL_Point* center, SDL_Rect* clipRect, SDL_RendererFlip flipType) {
	tex->center = center;
	tex->clipRect = clipRect;
	tex->flipType = flipType;
}

void TextureManager::loadTextureT(SDL_Renderer* renderer, TextureT* tex, const char* filename) {
	tex->texture = loadTexture(renderer, filename);
}

void TextureManager::textureRenderXYClip(SDL_Renderer* renderer, TextureT* texture, int x, int y, SDL_Rect* clipRect) {
	if(texture == NULL) {
		fmt::print("Passed texture was null! \n");
		return;
	}

	//texture->x = x;
	//texture->y = y;
	//texture->clipRect = clipRect;
	//SDL_Rect quad = {texture->x, texture->y, texture->width, texture->height};
	//if(texture->clipRect != NULL) {
	//	quad.w = texture->clipRect->w;
	//	quad.h = texture->clipRect->h;
	//}

	//SDL_RenderCopyEx(renderer, texture->texture, texture->clipRect, &quad, texture->angle, texture->center, texture->flipType);
}

void TextureManager::createRect(SDL_Rect* rect, int x, int y, int w, int h) {
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
}
