#pragma once

#include "Game.h"

class GameObject {
	public:
	GameObject(SDL_Renderer* renderer, const char* texturesheet, int xpos, int ypos, int width, int height);
	GameObject(SDL_Texture* texture, int xpos, int ypos, int width, int height);

	~GameObject();

	int getPosX();
	int getPosY();
	void move(int xpos, int ypos);
	void update();
	void render(SDL_Renderer* renderer);

	private:
	int xpos = 0;
	int ypos = 0;

	SDL_Texture* objTexture;
	SDL_Rect srcRect, destRect;
};