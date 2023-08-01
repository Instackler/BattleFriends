#pragma once
#include <BFconstants.h>

namespace BF
{
	class Entity : public sf::Sprite
	{
	public:
		//Entity(const char* filename);
		Entity(int textureID);
		Entity() = delete;
		Entity(Entity&) = delete;
		~Entity();
		Entity(Entity&& other) noexcept;
		Entity& operator= (const Entity&);
		Entity& operator=(const Entity&&) noexcept;

		void bounce();
		void setSpeed(float x, float y);
		virtual void update();
		virtual void collide(Entity& other);
		bool intersects(const Entity& other);
		friend void checkCollisions();
		friend void drawEntities(sf::RenderTarget& target);
		friend class Projectile;
		bool is_dead();
		bool out_of_bounds(sf::FloatRect area = sf::FloatRect{0.f, 0.f, (float)MAP_WIDTH, (float)MAP_HEIGHT});

	protected:
		float radius = 1.f;
		//sf::Texture m_Texture;
		int m_textureID = 0;
		float m_SpeedX = 0.f, m_SpeedY = 0.f;
		int health = 100;

	public:
		bool stationary = false;
	};
}