#include <SFML/Graphics.hpp>
#include <BF.h>
#include <loadTexture.h>
#include <iostream>

#define ENTITY_NUM 9




int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!", sf::Style::Close);
	window.setFramerateLimit(120);

	
	std::unique_ptr player = std::make_unique<BF::Player>("resources/logo.png");
	player->move(220.f, 101.f);
	std::vector<std::unique_ptr<BF::Entity>> entities;
	srand(time(NULL));
	for (int i = 0; i < ENTITY_NUM; i++)
	{
		entities.push_back(std::make_unique<BF::Entity>("resources/logo.png"));
		if (i < 4)
		{
			entities[i]->move((i + 2) * 220.f, 100.f);
			entities[i]->setSpeed(((rand() % 11) - 5) / 20.0f, (rand() % 11) / 10.0f);
		}
		else
		{
			entities[i]->move((i - 3) * 210.f, 600.f);
			entities[i]->setSpeed(((rand() % 11) - 5) / 20.0f, ((rand() % 11) - 10) / 10.0f);
		}
	}
	entities[1]->stationary = true;
	entities[5]->stationary = true;

	sf::Font font;
	font.loadFromFile("resources/fonts/raleway/Raleway-SemiBold.ttf");
	sf::Text text;
	text.setFont(font);
	text.setString("Hello world");

	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		
		//update entities
		BF::updateEntities();

		//check collisions
		BF::checkCollisions();

		//draw entities
		BF::drawEntities(window);

		window.draw(text);

		window.display();

		text.setString(std::to_string(1.f / clock.restart().asSeconds()));
	}

	return 0;
}