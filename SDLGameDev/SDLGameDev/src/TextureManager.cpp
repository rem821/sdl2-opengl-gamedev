#include "TextureManager.h"

SDL_Texture* TextureManager::loadTexture(const char* filename) {
	SDL_Surface* tmpSurface = IMG_Load(filename);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	return tex;
}

void TextureManager::draw(SDL_Texture* tex, SDL_Rect srcRect, SDL_Rect destRect) {
	SDL_RenderCopy(Game::renderer, tex, &srcRect, &destRect);
}
