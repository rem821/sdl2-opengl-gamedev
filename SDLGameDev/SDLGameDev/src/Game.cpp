#include "Game.h"

const int playerNum = 10000;
GameObject* players[playerNum];

Game::Game()
	{
	}

Game::~Game()
	{
	}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
	{
	int flags = 0;
	if(fullscreen)
		{
		flags = SDL_WINDOW_FULLSCREEN;
		}

	if(SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		fmt::print("Subsystem initialized!...\n");

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if(window)
			{
			fmt::print("Window created!...\n");
			}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if(renderer)
			{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			fmt::print("Renderer created!...\n");
			}

		isRunning = true;
		}
	else
		{
		isRunning = false;
		}

	SDL_Texture* text = TextureManager::loadTexture("assets/player.png", renderer);
	for(int i = 0; i < playerNum; i++)
		{
		players[i] = new GameObject(text, renderer);
		}
	}

void Game::handleEvents()
	{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch(event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym) {
				case SDLK_LEFT:
					//destRect.x -= 10;
					break;
				case SDLK_RIGHT:
					//destRect.x += 10;
					break;
				case SDLK_UP:
					//destRect.y -= 10;
					break;
				case SDLK_DOWN:
					//destRect.y += 10;
					break;
				default:
					break;
				}
			break;
		default:
			break;
		}

	}

void Game::update()
	{
	counter++;

	size_t numObjects = sizeof(players) / sizeof(players[0]);
	for(size_t i = 0; i < numObjects; ++i)
		{
		fmt::print("Updating player: {}\n", i);
		players[i]->update();
		}

	}

void Game::render()
	{
	SDL_RenderClear(renderer);

	size_t numObjects = sizeof(players) / sizeof(players[0]);
	for(size_t i = 0; i < numObjects; ++i)
		{
		players[i]->render();
		}

	SDL_RenderPresent(renderer);
	}

void Game::clean()
	{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	fmt::print("Game cleared!...\n");
	}