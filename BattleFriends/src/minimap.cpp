#include <pch.h>
#include <minimap.h>
#include <BF.h>


namespace 
{
	#ifdef _DEBUG
	sf::RenderTexture& get_texture()    //I have to use this because sf::RenderTexture doesn't support global initialization in debug mode
	{
		static sf::RenderTexture texture;
		return texture;
	}
	#define texture get_texture()
	#else
	static sf::RenderTexture texture;
	#endif // _DEBUG

	static sf::Sprite minimap_sprite;
}

void minimap::init()
{
	unsigned int screen_width = sf::VideoMode::getDesktopMode().width;
	unsigned int screen_height = sf::VideoMode::getDesktopMode().height;
	float offset = screen_height * MINIMAP_OFFSET;

	texture.create(MAP_WIDTH, MAP_HEIGHT); //TODO: add error checking
	texture.setSmooth(true);
	minimap_sprite.setTexture(texture.getTexture());
	minimap_sprite.setPosition(screen_width - MINIMAP_WIDTH - offset, offset);
	minimap_sprite.setScale(MINIMAP_SCALE, MINIMAP_SCALE);
	minimap_sprite.setColor(sf::Color(255, 255, 255, 100));
}

void minimap::draw(sf::RenderTarget& target)
{
	texture.clear();
	BF::drawEntities(texture);
	BF::drawPlayers(texture);
	texture.display();
	target.draw(minimap_sprite);
}
