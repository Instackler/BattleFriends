#pragma once

#define ENTITY_NUM 100
#define PLAYER_COUNT 2
const int MAP_WIDTH = 8000;
const int MAP_HEIGHT = 8000;
const float MINIMAP_SCALE = 0.05f / 1080.f;  // in fraction of map size
#define MINIMAP_WIDTH (int) MAP_WIDTH * MINIMAP_SCALE   // in pixels
#define MINIMAP_HEIGHT (int) MAP_HEIGHT * MINIMAP_SCALE  // in pixels
const float MINIMAP_OFFSET = 1.f / 54.f;   // in fraction of screen height
const float HUD_SCALE = 0.037f;
const int tick_rate = 200;
const int tick_time = 1000 / tick_rate; // in ms

//#define DISABLE_GGPO