#pragma once
#include <loadTexture.h>
#include <Entity.h>
#include <Player.h>
#include <Projectile.h>
#include <BFconstants.h>

#define SHOW_FPS


namespace BF
{
	extern std::vector<BF::Entity> entities;
	extern std::vector<BF::Player> players;
	extern std::vector<BF::Projectile> projectiles;

	void updateEntities();
	void updatePlayers();
	void updateProjectiles();

	void drawEntities(sf::RenderTarget& target);
	void drawPlayers(sf::RenderTarget& target);
	void drawProjectiles(sf::RenderTarget& target);

	void init();
	void update();
	void draw(sf::RenderTarget& target);
	
	void clear();

	void checkCollisions();
	void checkHits();

	void spawn_random_ent();

	size_t get_Entity_count();
	size_t get_Projectile_count();
}
