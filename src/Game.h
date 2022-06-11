#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_vulkan.h"
#include "CustomTypes.h"
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

	static SDL_Window* window;

	private:
	int counter = 0;
	bool isRunning = false;

	SDL_Renderer* renderer;
	SDL_Rect mouseRect;
	Point2DT cameraPos;
    int cameraRot = 0;
	int scrollAmount;
};