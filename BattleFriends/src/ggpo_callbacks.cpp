#include <pch.h>
#include <ggpo_callbacks.h>
#include <BF.h>

bool BF::begin_game(const char* game)
{
	return true;
}

bool BF::log_game_state(char* filename, unsigned char* buffer, int len)
{
	return true;
}

bool BF::advance_frame_callback(int flags)
{
	int disconnect_flags;
	ggpo_synchronize_input(session, game_inputs, PLAYER_COUNT * sizeof(player_inputs), &disconnect_flags);
	BF::advance();
	return true;
}

bool BF::on_event(GGPOEvent* info)
{
	return true;
}
