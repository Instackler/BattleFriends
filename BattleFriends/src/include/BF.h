#pragma once
#include <BFconstants.h>
#include <loadResource.h>
#include <Map.h>
#include <Entity.h>
#include <Player.h>
#include <Projectile.h>
#include <game_state.h>
#include <ggpo_callbacks.h>
	
namespace BF
{
	// game state and inputs
	extern std::vector<Entity> entities;
	extern std::vector<Player> players;
	extern std::vector<Projectile> projectiles;
	extern player_inputs game_inputs[PLAYER_COUNT];
	extern GGPOSession* session;

	//Non-game state
	extern std::vector<BF::Entity> map_objects;
	
	// renderer variables
	extern sf::RenderTarget* default_target;
	extern std::unordered_map<std::string, sf::Texture> textures;

	// game loop functions
	void init(sf::RenderTarget* target);
	void process_event(sf::Event& event, sf::RenderWindow& window);
	void advance();   //update w/o uploading local inputs, use only for ggpo advance_frame() callback
	void run();
	void draw(sf::RenderTarget& target = *default_target);
	void draw_debug_hud(sf::RenderTarget& target = *default_target);
	void clear();

	//ggpo callbacks
	bool save_game_state(unsigned char** buffer, int* len, int* checksum, int frame);
	void free_buffer(void* buffer);

	// utility functions
	void start_ggpo(int player_num);
	void spawn_random_ent();
	void loadTextures();
	void updateEntities();
	void updatePlayers();
	void updateProjectiles();
	void checkCollisions();
	void checkHits();
	void updateInputs();
	void physics_loop();
	size_t get_Entity_count();
	size_t get_Projectile_count();
	void drawMap(sf::RenderTarget& target = *default_target);
	void drawEntities(sf::RenderTarget& target = *default_target);
	void drawPlayers(sf::RenderTarget& target = *default_target);
	void drawProjectiles(sf::RenderTarget& target = *default_target);
}
