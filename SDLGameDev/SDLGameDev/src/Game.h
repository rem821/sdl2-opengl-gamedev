#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "SDL_image.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "fmt/core.h"

class Game
	{

	public:
		Game();
		~Game();

		void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
		
		void handleEvents();
		void update();
		void render();
		void clean();

		bool running() { return isRunning; };

	private:
		int counter = 0;
		bool isRunning = false;
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer= nullptr;

	};

#endif /* GAME_H */