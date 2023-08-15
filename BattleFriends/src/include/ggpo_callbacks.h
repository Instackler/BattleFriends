#pragma once

namespace BF
{
	bool begin_game(const char* game);
	bool log_game_state(char* filename, unsigned char* buffer, int len);
	bool advance_frame_callback(int flags);
	bool on_event(GGPOEvent* info);
}
