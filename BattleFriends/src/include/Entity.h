#pragma once
#include <BFconstants.h>
#include <Map.h>

namespace BF
{
	extern int MAP_WIDTH;
	extern int MAP_HEIGHT;

	class Entity : public sf::Sprite
	{
	public:
		Entity(const std::string& textureID);
		Entity() = delete;

		void setSpeed(float x, float y);
		void update();
		bool intersects(const Entity& other);
		bool intersects_map(const Entity& other);
		friend void checkCollisions();
		friend void drawEntities(sf::RenderTarget& target);
		friend class Projectile;
		friend class Player;
		bool is_dead();
		bool out_of_bounds(sf::FloatRect area = sf::FloatRect{0.f, 0.f, (float)BF::MAP_WIDTH, (float)BF::MAP_HEIGHT});
		void set_points();

	protected:
		sf::Vector2f points[4];
		float m_SpeedX = 0.f, m_SpeedY = 0.f;
		std::string m_textureID;
		float radius = 1.f;
		int health = 100;

	public:
		bool stationary = false;
	};
}