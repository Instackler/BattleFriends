#pragma once
#include <loadTexture.h>
#include <Entity.h>
#include <Player.h>
#include <Projectile.h>
#include <BFconstants.h>

#define SHOW_FPS

	
namespace BF
{
	extern std::vector<Entity> entities;
	extern std::vector<Player> players;
	extern std::vector<Projectile> projectiles;
	extern sf::RenderTarget* default_target;
	extern sf::VideoMode screen_params;

	void updateEntities();
	void updatePlayers();
	void updateProjectiles();

	void drawEntities(sf::RenderTarget& target = *default_target);
	void drawPlayers(sf::RenderTarget& target = *default_target);
	void drawProjectiles(sf::RenderTarget& target = *default_target);

	void init(sf::RenderTarget* target);
	void update();
	void draw(sf::RenderTarget& target = *default_target);
	
	void clear();

	void checkCollisions();
	void checkHits();

	void spawn_random_ent();

	size_t get_Entity_count();
	size_t get_Projectile_count();
}
