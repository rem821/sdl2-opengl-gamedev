#include "Game.h"

Game *game = nullptr;

int main(int args, char *argv[]) {
    game = new Game();
    game->~Game();
    return 0;
}