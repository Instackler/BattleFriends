#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

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
		friend void bounce(Entity& a, Entity& b);
		bool intersects(const Entity& other);
		friend void updateEntities();
		friend void checkCollisions();
		bool stationary = false;

	private:
		static int s_num;
		float m_SpeedX = 0.f, m_SpeedY = 0.f;
		float m_mass = 1.f;
		float radius = 1.f;

	protected:
		bool m_collided = false;
		sf::Texture m_texture;
	};

	void updateEntities();
	void drawEntities(sf::RenderTarget& target);
	void checkCollisions();


	class Player : public Entity
	{
	public:
		Player(const char* filename);
		~Player();

		void update() override;

	private:
		bool m_changedDirecton = true;

	};
}