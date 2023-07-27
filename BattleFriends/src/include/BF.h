#pragma once
#include <loadTexture.h>
#include <Entity.h>
#include <Player.h>
#include <Projectile.h>
#include <BFconstants.h>

#define SHOW_FPS

	
namespace BF
{
	extern sf::RenderTarget* default_target;
	extern std::vector<BF::Entity> entities;
	extern std::vector<BF::Player> players;
	extern std::vector<BF::Projectile> projectiles;
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
