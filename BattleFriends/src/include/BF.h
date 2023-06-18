#pragma once
#include <SFML/Graphics.hpp>

namespace BF 
{
	class Entity : public sf::Sprite
	{
	private:
		sf::Texture m_texture;

	public:
		Entity(std::string& filename);
	};
}