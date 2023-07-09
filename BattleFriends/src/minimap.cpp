#include <pch.h>
#include <minimap.h>
#include <BF.h>


namespace 
{
	#ifdef _DEBUG
	sf::RenderTexture& get_texture()    //I have to use this because sf::RenderTexture doesn't properly support global initialization in debug mode
	{
		static sf::RenderTexture texture;
		return texture;
	}
	#define texture get_texture()
	#else
	static sf::RenderTexture texture;
	#endif // _DEBUG
	
	/*
	sf::RenderTexture& get_texture()
	{
		static sf::RenderTexture texture;
		return texture;
	}
	*/

	static sf::Sprite sprite;
	static sf::RectangleShape background(sf::Vector2f(MAP_WIDTH, MAP_HEIGHT));
}


void minimap::init()
{
	texture.create(5000, 5000); //add error checking
	texture.setSmooth(true);
	sprite.setTexture(texture.getTexture());
	sprite.setPosition(1400.f, 20.f);
	sprite.setScale(0.06f, 0.06f);
	sprite.setColor(sf::Color(255, 255, 255, 100));
}

void minimap::update()
{
	texture.clear();
	BF::draw_world(texture);
	texture.display();
}

void minimap::draw(sf::RenderTarget& target)
{
	update();
	target.draw(sprite);
}
