#include <pch.h>
#include <game_state.h>
#include <BF.h>

BF::game_state::game_state(size_t a, size_t b, size_t c)
	:counts(a, b, c),
	counts_data(&counts),
	entity_data(entities.data()),
	player_data(players.data()),
	projectile_data(projectiles.data())
{
	counts_byte_size = sizeof(counts);
	entities_byte_size = counts.entity_count * sizeof(Entity);
	players_byte_size = counts.player_count * sizeof(Player);
	projectiles_byte_size = counts.projectile_count * sizeof(Projectile);
	total_byte_size = counts_byte_size + entities_byte_size + players_byte_size + projectiles_byte_size;

	counts_offset = 0;
	entities_offset = counts_byte_size;
	players_offset = entities_offset + entities_byte_size;
	projectiles_offset = players_offset + players_byte_size;
}

bool BF::save_game_state(unsigned char** buffer, int* len, int* checksum, int frame)
{
	game_state gs(entities.size(), players.size(), projectiles.size());
	*len = gs.total_byte_size;
	*buffer = new unsigned char[*len];
	std::memcpy(*buffer + gs.counts_offset, gs.counts_data, gs.counts_byte_size);
	std::memcpy(*buffer + gs.entities_offset, gs.entity_data, gs.entities_byte_size);
	std::memcpy(*buffer + gs.players_offset, gs.player_data, gs.players_byte_size);
	std::memcpy(*buffer + gs.projectiles_offset, gs.projectile_data, gs.projectiles_byte_size);
	return true;
}

bool BF::load_game_state(unsigned char* buffer, int len)
{
	size_t3 counts{};
	std::memcpy(&counts, buffer, sizeof(counts));
	game_state gs{ counts.entity_count, counts.player_count, counts.projectile_count };
	entities = std::vector<Entity>((Entity*)(buffer + gs.entities_offset), (Entity*)(buffer + gs.entities_offset) + gs.counts.entity_count);
	players = std::vector<Player>((Player*)(buffer + gs.players_offset), (Player*)(buffer + gs.players_offset) + gs.counts.player_count);
	projectiles = std::vector<Projectile>((Projectile*)(buffer + gs.projectiles_offset), (Projectile*)(buffer + gs.projectiles_offset) + gs.counts.projectile_count);
	return true;
}

void BF::free_buffer(void* buffer)
{
	delete[] buffer;
}
