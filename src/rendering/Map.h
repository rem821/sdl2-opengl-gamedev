#pragma once

#include "VulkanEngineDevice.h"
#include "Cube.h"
#include "GameObject.h"

#include "fmt/core.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define MAP_HEIGHT 100
#define MAP_WIDTH 100
#define MAP_DEPTH 20

class Map {
public:
    Map(VulkanEngineDevice &engineDevice, Cube &cube);
    ~Map();

    GameObject::Map getMapBlocks();

private:
    void loadLevel(int level);

    VulkanEngineDevice &device;
    Cube &_cube;

    uint32_t map_blocks[MAP_DEPTH][MAP_WIDTH][MAP_HEIGHT];
    int level[MAP_DEPTH][MAP_WIDTH][MAP_HEIGHT];
};