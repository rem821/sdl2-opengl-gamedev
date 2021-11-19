#include "Game.h"

Map* map;
GameObject* player;

SDL_Window* Game::window = nullptr;

Game::Game() {}

Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	int flags = 0;
	if(fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	} else {
		flags = SDL_WINDOW_RESIZABLE;
	}

	if(SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		fmt::print("Subsystem initialized!...\n");

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if(window) {
			fmt::print("Window created!...\n");
		}
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2")) {
			fmt::print("Warning: Linear texture filtering was not enabled!...\n");
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if(renderer) {
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
			fmt::print("Renderer created!...\n");
		}

		isRunning = true;
	} else {
		isRunning = false;
	}

	map = new Map(renderer, false);
	player = new GameObject(renderer, "assets/player.png", 0, 0, 64, 64);
}

void Game::handleEvents() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch(event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_MOUSEWHEEL:
		if(event.wheel.y > 0) {
			++scrollAmount;
		} else if(event.wheel.y < 0) {
			--scrollAmount;
		}
	case SDL_MOUSEBUTTONDOWN:
		if(event.button.button == SDL_BUTTON_LEFT) {
			map->onMapTileClick(mouseRect, cameraPos);
		}
	default:
		break;
	}

	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	if(keystate[SDL_SCANCODE_W]) {
		cameraPos.y -= 10;
	}
	if(keystate[SDL_SCANCODE_A]) {
		cameraPos.x -= 10;
	}
	if(keystate[SDL_SCANCODE_S]) {
		cameraPos.y += 10;
	}
	if(keystate[SDL_SCANCODE_D]) {
		cameraPos.x += 10;
	}

	SDL_GetMouseState(&mouseRect.x, &mouseRect.y);
}

void Game::update() {
	counter++;
	map->updateIsoMap(cameraPos, scrollAmount);
	player->update();
}

void Game::render() {
	SDL_RenderClear(renderer);

	map->drawIsoMap();
	//map->drawCursor(mouseRect);
	map->drawIsoCursor(mouseRect, cameraPos);
	//player->render(renderer);

	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	fmt::print("Game cleared!...\n");
}