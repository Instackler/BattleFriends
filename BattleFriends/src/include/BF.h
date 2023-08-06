#pragma once
#include <BFconstants.h>
#include <loadResource.h>
#include <Entity.h>
#include <Player.h>
#include <Projectile.h>
	
namespace BF
{
	// game state and inputs
	extern std::vector<Entity> entities;
	extern std::vector<Player> players;
	extern std::vector<Projectile> projectiles;
	extern std::vector<BF::player_inputs> game_inputs;

	// renderer variables
	extern sf::RenderTarget* default_target;
	extern std::unordered_map<int, sf::Texture> textures;

	extern std::mutex update_mutex;

	// game loop functions
	void init(sf::RenderTarget* target);
	void process_event(sf::Event& event, sf::RenderWindow& window);
	void update();
	void draw(sf::RenderTarget& target = *default_target);
	void draw_debug_hud(sf::RenderTarget& target = *default_target);
	void clear();

	//ggpo callbacks
	bool save_game_state(unsigned char** buffer, int* len, int* checksum, int frame);
	void free_buffer(void* buffer);

	// utility functions
	void spawn_random_ent();
	void loadTextures();
	void updateEntities();
	void updatePlayers();
	void updateProjectiles();
	void checkCollisions();
	void checkHits();
	void checkInputs();
	void physics_loop();
	size_t get_Entity_count();
	size_t get_Projectile_count();
	void drawEntities(sf::RenderTarget& target = *default_target);
	void drawPlayers(sf::RenderTarget& target = *default_target);
	void drawProjectiles(sf::RenderTarget& target = *default_target);
}
