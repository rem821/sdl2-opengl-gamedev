#include "Map.h"

Map::Map(SDL_Renderer *renderer) {
    this->renderer = renderer;

    for (int tile = 0; tile < NUM_ISOMETRIC_TILES; ++tile) {
        TextureManager::initTextureT(&tilesTex[tile], NULL, NULL, SDL_FLIP_NONE);
        TextureManager::loadTextureT(renderer, &tilesTex[tile],
                                     fmt::format("assets/iso2/blocks_{}.png", tile + 0).c_str());
    }

    for (int z = 0; z < MAP_DEPTH; ++z) {
        loadLevel(z);
        for (int x = 0; x < MAP_WIDTH; ++x) {
            for (int y = 0; y < MAP_HEIGHT; ++y) {
                tiles[z][x][y].pos = new Point2DT();

                if (level[z][x][y] == 1) {
                    std::random_device rd;
                    std::mt19937 mt(rd());
                    std::uniform_real_distribution<double> dist(44.0, 46.0);
                    tiles[z][x][y].texture = &tilesTex[int(dist(mt))];
                    tiles[z][x][y].isBlock = true;
                } else {
                    tiles[z][x][y].isBlock = false;
                }
            }
        }
    }
}

Map::~Map() {}

void Map::updateIsoMap(SDL_Rect mouseRect, Point2DT cameraPos, int scrollAmount) {
    TILE_OUTPUT_SIZE = 256 + scrollAmount;
    highlightedTile.x = -1;
    highlightedTile.y = -1;
    highlightedTile.z = -1;

    for (int z = MAP_DEPTH - 1; z >= 0; --z) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            for (int y = 0; y < MAP_HEIGHT; ++y) {
                tiles[z][x][y].pos->x = x * TILE_OUTPUT_SIZE;
                tiles[z][x][y].pos->y = y * TILE_OUTPUT_SIZE;
                tiles[z][x][y].zLevel = z;

                IsoEngine::convert2DToIso(tiles[z][x][y].pos, z, TILE_OUTPUT_SIZE);

                tiles[z][x][y].isBlock = level[z][x][y] == 1;

                //No tile has been highlighted yet
                if (highlightedTile.x == -1 && highlightedTile.y == -1 && tiles[z][x][y].isBlock) {

                    //Cursor position in absolute world coordinates
                    Point2DT cursorAbsPos;
                    cursorAbsPos.x = mouseRect.x + cameraPos.x;
                    cursorAbsPos.y = mouseRect.y + cameraPos.y;

                    //Highlight if the tile is hovered over
                    highlightIfSelected(cursorAbsPos, x, y, z);
                }
            }
        }
    }
}

void Map::drawIsoMap(Point2DT cameraPos, int cameraRot) {
    for (int z = 0; z < MAP_DEPTH; ++z) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            for (int y = 0; y < MAP_HEIGHT; ++y) {
                if (tiles[z][x][y].isBlock) {
                    SDL_Rect srcRect;
                    srcRect.x = 0;
                    srcRect.y = 0;
                    srcRect.w = TILE_INPUT_SIZE;
                    srcRect.h = TILE_INPUT_SIZE;

                    SDL_Rect destRect;
                    destRect.x = tiles[z][x][y].pos->x - cameraPos.x;
                    destRect.y = tiles[z][x][y].pos->y - cameraPos.y;
                    destRect.w = TILE_OUTPUT_SIZE;
                    destRect.h = TILE_OUTPUT_SIZE;

                    TextureManager::draw(renderer, tiles[z][x][y].texture->texture, srcRect, destRect, cameraRot);

                    if (highlightedTile.z == z && highlightedTile.x == x && highlightedTile.y == y) {
                        drawIsoCursor(cameraPos, cameraRot);
                    }
                }
            }
        }
    }
}

void Map::drawCursor(SDL_Rect mouseRect) {
    SDL_Rect srcRect;
    srcRect.x = TILE_INPUT_SIZE;
    srcRect.y = TILE_INPUT_SIZE;
    TextureManager::draw(renderer, tilesTex[0].texture, srcRect, mouseRect, 0);
}

void Map::drawIsoCursor(Point2DT cameraPos, int cameraRot) {
    Point2DT worldPos;

    if (highlightedTile.x < 0 || highlightedTile.y < 0 || highlightedTile.x >= MAP_WIDTH ||
        highlightedTile.y >= MAP_HEIGHT)
        return;

    SDL_Rect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = TILE_INPUT_SIZE;
    srcRect.h = TILE_INPUT_SIZE;

    SDL_Rect destRect;
    destRect.x = tiles[highlightedTile.z][highlightedTile.x][highlightedTile.y].pos->x - cameraPos.x;
    destRect.y = tiles[highlightedTile.z][highlightedTile.x][highlightedTile.y].pos->y - cameraPos.y;
    destRect.w = TILE_OUTPUT_SIZE;
    destRect.h = TILE_OUTPUT_SIZE;

    TextureManager::draw(renderer, tilesTex[0].texture, srcRect, destRect, cameraRot);
    //fmt::print("highlighting new tile: {} {}\n", highlightedTile.x, highlightedTile.y);
}

void Map::onMapTileClick() {

    if (highlightedTile.x < 0 || highlightedTile.y < 0 || highlightedTile.x >= MAP_WIDTH ||
        highlightedTile.y >= MAP_HEIGHT || highlightedTile.z == 0)
        return;

    level[highlightedTile.z][highlightedTile.x][highlightedTile.y] =
            level[highlightedTile.z][highlightedTile.x][highlightedTile.y] != 1;
}

void Map::drawDot(SDL_Rect mouseRect, int type) {
    SDL_Rect srcRect;
    srcRect.w = TILE_INPUT_SIZE;
    srcRect.h = TILE_INPUT_SIZE;
    srcRect.x = 0;
    srcRect.y = 0;

    mouseRect.x -= 10;
    mouseRect.y -= 10;
    mouseRect.w = 20;
    mouseRect.h = 20;

    TextureManager::draw(renderer, tilesTex[type].texture, srcRect, mouseRect, 0);
}

void Map::loadLevel(int z) {
    std::ifstream infile(fmt::format("assets/map/level_{}.txt", z));

    int x = 0;
    int y = 0;

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);

        for (int i = 0; i < line.size(); ++i) {
            char ch = line[i];

            if (ch == '1' || ch == '0') {
                level[z][x][y] = ch - '0';
                x += 1;
            }
        }
        y += 1;
        x = 0;
    }

    infile.close();
    //fmt::print(fmt::format("Level {} processed\n", z));
}

void Map::highlightIfSelected(Point2DT cursorAbsPos, int x, int y, int z) {

    //Tile bounds
    int startX = tiles[z][x][y].pos->x;
    int startY = tiles[z][x][y].pos->y;
    int endX = startX + TILE_OUTPUT_SIZE;
    int endY = startY + TILE_OUTPUT_SIZE / 2;

    //Cursor is inside the tile bounds
    if (cursorAbsPos.x >= startX && cursorAbsPos.x <= endX && cursorAbsPos.y >= startY && cursorAbsPos.y <= endY) {
        //Cursor position relative to the tile
        Point2DT tilePos;
        tilePos.x = cursorAbsPos.x - startX;
        tilePos.y = cursorAbsPos.y - startY;

        if ((tilePos.y <= TILE_OUTPUT_SIZE / 4 &&         //Upper left half of the tile top side
             tilePos.x <= TILE_OUTPUT_SIZE / 2 &&
             tilePos.y > (TILE_OUTPUT_SIZE / 4) - (tilePos.x / 2))
            ||
            (tilePos.y <= TILE_OUTPUT_SIZE / 4 &&         //Upper right half of the tile top side
             tilePos.x > TILE_OUTPUT_SIZE / 2 &&
             tilePos.y > ((tilePos.x - (TILE_OUTPUT_SIZE / 2)) / 2))
            ||
            (tilePos.y > TILE_OUTPUT_SIZE / 4 &&         //Lower left half of the tile top side
             tilePos.x <= TILE_OUTPUT_SIZE / 2 &&
             tilePos.y < (TILE_OUTPUT_SIZE / 4) + (tilePos.x / 2))
            ||
            (tilePos.y > TILE_OUTPUT_SIZE / 4 &&         //Lower right half of the tile top side
             tilePos.x > TILE_OUTPUT_SIZE / 2 &&
             tilePos.y < ((TILE_OUTPUT_SIZE / 2) - ((tilePos.x - (TILE_OUTPUT_SIZE / 2)) / 2)))) {
            highlightedTile.x = x;
            highlightedTile.y = y;
            highlightedTile.z = z;
        }

        if (highlightedTile.x != -1 && highlightedTile.y != -1 && highlightedTile.z != -1 &&
            highlightedTile.z != MAP_DEPTH - 1) {
            //If the tile above is block, selection is invalid
            if (tiles[highlightedTile.z + 1][highlightedTile.x][highlightedTile.y].isBlock) {
                highlightedTile.x = -1;
                highlightedTile.y = -1;
                highlightedTile.y = -1;
            }
        }
    }
}
