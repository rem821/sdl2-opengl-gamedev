#include "GameObject.h"
#include "TextureManager.h"
#include "fmt/core.h"

GameObject::GameObject(const char* texturesheet, SDL_Renderer* renderer)
	{
	this->renderer = renderer;
	objTexture = TextureManager::loadTexture(texturesheet, renderer);

	xpos = rand() % 800;
	ypos = rand() % 600;
	srcRect.h = 64;
	srcRect.w = 64;
	srcRect.x = 0;
	srcRect.y = 0;
	}

GameObject::GameObject(SDL_Texture* texture, SDL_Renderer* renderer)
	{
	this->renderer = renderer;
	objTexture = texture;

	xpos = rand() % 800;
	ypos = rand() % 600;
	srcRect.h = 64;
	srcRect.w = 64;
	srcRect.x = 0;
	srcRect.y = 0;
	}

GameObject::~GameObject()
	{
	}

void GameObject::update()
	{
	int xRand = rand() % 100;
	int yRand = rand() % 100;
	if(xRand > 50)
		{
		xpos += 10;
		}
	else
		{
		xpos -= 10;
		}
	if(xpos > 800) xpos = 800;
	if(xpos < 0) xpos = 0;

	if(yRand > 50)
		{
		ypos += 10;
		}
	else
		{
		ypos -= 10;
		}
	if(ypos > 600) ypos = 600;
	if(ypos < 0) ypos = 0;

	destRect.x = xpos;
	destRect.y = ypos;

	fmt::print("New pos for player x:{} y:{}\n", xpos, ypos);
	destRect.w = srcRect.w;
	destRect.h = srcRect.h;
	}

void GameObject::render()
	{
	SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
	}
