#pragma once

#define ENTITY_NUM 100
#define PLAYER_COUNT 2
const int MAP_WIDTH = 8000;
const int MAP_HEIGHT = 8000;
const float MINIMAP_SCALE = 0.35f;  // in fraction of screen height
const float MINIMAP_OFFSET = MINIMAP_SCALE * 0.03f;   // in fraction of screen height
const float HUD_SCALE = 0.037f;
const int tick_rate = 200;
const int tick_time = 1000 / tick_rate; // in ms

#define DISABLE_GGPO

#ifdef DISABLE_GGPO
#define PLAYER_COUNT 1
#endif // DISABLE_GGPO
