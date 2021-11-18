#include "GameObject.h"
#include "TextureManager.h"
#include "fmt/core.h"

GameObject::GameObject(SDL_Renderer* renderer, const char* texturesheet, int xpos, int ypos, int width, int height) {
	objTexture = TextureManager::loadTexture(renderer, texturesheet);
	this->xpos = xpos;
	this->ypos = ypos;
	srcRect.h = height;
	srcRect.w = width;
	srcRect.x = 0;
	srcRect.y = 0;
}

GameObject::GameObject(SDL_Texture* texture, int xpos, int ypos, int width, int height) {
	objTexture = texture;
	this->xpos = xpos;
	this->ypos = ypos;
	srcRect.h = height;
	srcRect.w = width;
	srcRect.x = 0;
	srcRect.y = 0;
}

GameObject::~GameObject() {
}

int GameObject::getPosX() {
	return this->xpos;
}

int GameObject::getPosY() {
	return this->ypos;
}

void GameObject::move(int xpos, int ypos) {
	this->xpos = xpos;
	this->ypos = ypos;
}

void GameObject::update() {
	if(this->xpos > 1600) this->xpos = 1600;
	if(this->xpos < 0) this->xpos = 0;

	if(this->ypos > 1200) this->ypos = 1200;
	if(this->ypos < 0) this->ypos = 0;

	destRect.x = xpos;
	destRect.y = ypos;

	destRect.w = srcRect.w;
	destRect.h = srcRect.h;
}

void GameObject::render(SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}
