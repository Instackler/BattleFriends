#pragma once
#include <loadResource.h>
#include <Entity.h>
#include <Player.h>
#include <Projectile.h>
#include <BFconstants.h>

//#define SHOW_FPS

	
namespace BF
{
	// game state
	extern std::vector<Entity> entities;
	extern std::vector<Player> players;
	extern std::vector<Projectile> projectiles;

	// game inputs
	extern std::vector<BF::player_inputs> game_inputs;

	extern sf::RenderTarget* default_target;
	extern sf::VideoMode screen_params;
	extern std::unordered_map<int, sf::Texture> textures;
	extern std::atomic_flag running;
	extern std::atomic<sf::Time> physics_time;

	void loadTextures();

	void updateEntities();
	void updatePlayers();
	void updateProjectiles();
	void checkCollisions();
	void checkHits();
	void checkInputs();

	void drawEntities(sf::RenderTarget& target = *default_target);
	void drawPlayers(sf::RenderTarget& target = *default_target);
	void drawProjectiles(sf::RenderTarget& target = *default_target);

	void init(sf::RenderTarget* target);
	void physics_loop();
	void update();
	void draw(sf::RenderTarget& target = *default_target);
	void draw_debug_hud(sf::RenderTarget& target = *default_target);
	void clear();

	bool save_game_state(unsigned char** buffer, int* len, int* checksum, int frame);
	void free_buffer(void* buffer);

	void spawn_random_ent();

	size_t get_Entity_count();
	size_t get_Projectile_count();
}
