#pragma once
#include <BFconstants.h>

namespace BF
{
	class Entity : public sf::Sprite
	{
	public:
		Entity(const std::string& textureID);
		Entity(const Entity&);
		Entity() = delete;
		Entity(Entity&& other) noexcept;
		Entity& operator=(const Entity&);
		Entity& operator=(Entity&&) noexcept;

		void bounce();
		void setSpeed(float x, float y);
		virtual void update();
		bool intersects(const Entity& other);
		bool intersects_map(const Entity& other);
		friend void checkCollisions();
		friend void drawEntities(sf::RenderTarget& target);
		friend class Projectile;
		friend class Player;
		bool is_dead();
		bool out_of_bounds(sf::FloatRect area = sf::FloatRect{0.f, 0.f, (float)MAP_WIDTH, (float)MAP_HEIGHT});
		bool set_radius();

	protected:
		float m_SpeedX = 0.f, m_SpeedY = 0.f;
		std::string m_textureID;
		float radius = 1.f;
		int health = 100;

	public:
		bool stationary = false;
	};
}