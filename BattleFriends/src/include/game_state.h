#pragma once

namespace BF
{
	struct size_t3
	{
		size_t entity_count = 0;
		size_t player_count = 0;
		size_t projectile_count = 0;

		size_t3(size_t a, size_t b, size_t c)
			:entity_count(a), player_count(b), projectile_count(c) {}
		size_t3() {};
	};

	struct game_state
	{

		game_state(size_t a, size_t b, size_t c);
		game_state() = delete;

		size_t total_byte_size;
		size_t counts_byte_size;
		size_t entities_byte_size;
		size_t players_byte_size;
		size_t projectiles_byte_size;

		size_t counts_offset;
		size_t entities_offset;
		size_t players_offset;
		size_t projectiles_offset;

		size_t3 counts;

		void* counts_data = nullptr;
		void* entity_data = nullptr;
		void* player_data = nullptr;
		void* projectile_data = nullptr;
	};

	bool save_game_state(unsigned char** buffer, int* len, int* checksum, int frame);
	bool load_game_state(unsigned char* buffer, int len);
	void free_buffer(void* buffer);
}
