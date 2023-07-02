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
		bool m_collided = false;
		sf::Texture m_texture;
		float m_SpeedX = 0.f, m_SpeedY = 0.f;
		int health = 100;

	public:
		bool stationary = false;
	};

	void updateEntities();
	void drawEntities(sf::RenderTarget& target);
	void checkCollisions();
	void checkHits();

	enum dir
	{
		up, upleft, left, downleft, down, downright, right, upright
	};


	class Player : public Entity
	{
	public:
		Player(const char* filename);
		~Player();

		void update() override;

	private:
		void checkInput();
		void shoot();

	private:
		dir m_dir = up;
	};

	
	class Projectile : public Entity
	{
	public:
		Projectile();
		void collide(Entity& other);

	private:
		int damage = 10;
	};
}