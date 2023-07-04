#pragma once

namespace BF
{
	class Entity : public sf::Sprite
	{
	public:
		Entity(const char* filename);
		Entity() = delete;
		~Entity();
		Entity(Entity&& other) noexcept;
		Entity& operator= (const Entity&) = default;
		void setSpeed(float x, float y);
		virtual void update();
		virtual void collide(Entity& other);
		bool intersects(const Entity& other);
		friend void updateEntities();
		friend void checkCollisions();
		friend void drawEntities(sf::RenderTarget& target);
		friend class Projectile;


	private:
		static int s_num;
		float radius = 1.f;

	protected:
		sf::Texture m_Texture;
		float m_SpeedX = 0.f, m_SpeedY = 0.f;
		int health = 100;

	public:
		bool stationary = false;
	};
}