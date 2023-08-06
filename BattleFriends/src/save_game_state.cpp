#include <pch.h>
#include <save_game_state.h>
#include <BF.h>

bool BF::save_game_state(unsigned char** buffer, int* len, int* checksum, int frame)
{
	const int NUMBER_OF_CONTAINERS = 3;
	const std::lock_guard<std::mutex> ggpo_lock(update_mutex);
	int entities_offset = entities.size() * sizeof(Entity);
	int players_offset = players.size() * sizeof(Player);
	int projectiles_offset = projectiles.size() * sizeof(Projectile);
	int sizes_offset = sizeof(size_t) * NUMBER_OF_CONTAINERS;
	size_t sizes[NUMBER_OF_CONTAINERS]{ entities.size(), players.size(), projectiles.size() };

	*len = sizes_offset + entities_offset + players_offset + projectiles_offset;
	*buffer = new unsigned char[*len];
	size_t it = 0;

	std::memcpy(*buffer, sizes, sizes_offset);
	it += sizes_offset;
	std::memcpy(*buffer + it, entities.data(), entities_offset);
	it += entities_offset;
	std::memcpy(*buffer + it, players.data(), players_offset);
	it += players_offset;
	std::memcpy(*buffer + it, projectiles.data(), projectiles_offset);

	return true;
}