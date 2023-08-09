#include <pch.h>
#include <ggpo_callbacks.h>

bool BF::begin_game(const char* game)
{
    return true;
}

bool BF::log_game_state(char* filename, unsigned char* buffer, int len)
{
    return true;
}

bool BF::advance_frame(int flags)
{
    return false;
}
