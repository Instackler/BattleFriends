#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

namespace BF
{
	

	class Entity : public sf::Sprite
	{
	public:
		Entity(const char* filename);
		Entity();
		~Entity();
		Entity(const Entity&) = delete;
		Entity& operator= (const Entity&) = delete;
		void loadTexture(const char* filename);
		void setTexture();
		void setSpeed(float x, float y);
		virtual void update();
		friend void bounce(Entity& a, Entity& b);

	private:
		static int num;
		float m_SpeedX = 0, m_SpeedY = 0;
		bool m_collided = false;
		sf::Texture m_texture;
	};

	void update_Entities();
	void draw_Entities(sf::RenderTarget& target);
	void check_collisions();

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