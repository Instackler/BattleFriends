#pragma once
#include <SFML/Graphics.hpp>

namespace BF 
{
	class Entity : public sf::Sprite
	{
	private:
		sf::Texture m_texture;
		float m_SpeedX, m_SpeedY;

	public:
		Entity(const char* filename);
		void setSpeed(float x, float y);
		void update();
		void bounce();
	};
}