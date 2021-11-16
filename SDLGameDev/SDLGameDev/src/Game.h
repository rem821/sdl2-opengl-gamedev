#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Map.h"
#include "fmt/core.h"

class Game {

	public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() { return isRunning; };

	static SDL_Renderer* renderer;

	private:
	int counter = 0;
	bool isRunning = false;
	SDL_Window* window = nullptr;
};