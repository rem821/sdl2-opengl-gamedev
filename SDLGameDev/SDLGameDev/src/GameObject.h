#pragma once
#include "Game.h"

class GameObject 
	{
	public:
		GameObject(const char* texturesheet, SDL_Renderer* renderer);
		GameObject(SDL_Texture* texture, SDL_Renderer* renderer);

		~GameObject();

		void update();
		void render();

	private:
		int xpos = 0;
		int ypos = 0;

		SDL_Texture* objTexture;
		SDL_Rect srcRect, destRect;
		SDL_Renderer* renderer;
	};