#include "Game.h"

Map* map;
GameObject* player;

SDL_Renderer* Game::renderer = nullptr;

Game::Game() {
}

Game::~Game() {
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	int flags = 0;
	if(fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if(SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		fmt::print("Subsystem initialized!...\n");

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if(window) {
			fmt::print("Window created!...\n");
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if(renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			fmt::print("Renderer created!...\n");
		}

		isRunning = true;
	} else {
		isRunning = false;
	}

	map = new Map();
	player = new GameObject("assets/player.png", 0, 0, 64, 64);
}

void Game::handleEvents() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch(event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_KEYDOWN:
		switch(event.key.keysym.sym) {
		case SDLK_LEFT:
			player->move(player->getPosX() - 10, player->getPosY());
			break;
		case SDLK_RIGHT:
			player->move(player->getPosX() + 10, player->getPosY());
			break;
		case SDLK_UP:
			player->move(player->getPosX(), player->getPosY() - 10);
			break;
		case SDLK_DOWN:
			player->move(player->getPosX(), player->getPosY() + 10);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

}

void Game::update() {
	counter++;
	player->update();
}

void Game::render() {
	SDL_RenderClear(renderer);

	map->drawMap();
	player->render();

	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	fmt::print("Game cleared!...\n");
}