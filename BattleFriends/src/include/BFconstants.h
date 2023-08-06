#pragma once

#define ENTITY_NUM 100
const int MAP_WIDTH = 8192;
const int MAP_HEIGHT = 8192;
const float MINIMAP_SCALE = 0.05f;  // in fraction of map size
#define MINIMAP_WIDTH (int) MAP_WIDTH * MINIMAP_SCALE   // in pixels
#define MINIMAP_HEIGHT (int) MAP_HEIGHT * MINIMAP_SCALE  // in pixels
const float MINIMAP_OFFSET = 1.f / 54.f;   // in fraction of screen height