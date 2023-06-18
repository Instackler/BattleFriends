#include <SFML/Graphics.hpp>
#include <BF.h>


int main()
{
	std::string fname = "resources/logo.png";
	bool check = 0;
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!", sf::Style::Close);
	//window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	BF::Entity player(fname);
	
	sf::Texture texture;
	texture.loadFromFile("resources/logo.png");
	sf::Sprite sprite;
	sprite.setTexture(texture);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(player);
		window.display();
	}

	return 0;
}