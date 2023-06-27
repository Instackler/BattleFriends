#include <SFML/Graphics.hpp>
#include <BF.h>
#include <iostream>
#define ENTITY_NUM 9


int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!", sf::Style::Close);
	//window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	/*BF::Entity player1("resources/logo.png");
	BF::Player player2("resources/logo.png");
	player1.setSpeed(0.f, 1.f);
	player2.move(0.f, 400.f);
	player2.setSpeed(0.f, -1.f);*/

	std::unique_ptr player = std::make_unique<BF::Player>("resources/logo.png");
	std::vector<std::unique_ptr<BF::Entity>> entities;
	for (int i = 0; i < ENTITY_NUM; i++)
	{
		entities.push_back(std::make_unique<BF::Entity>("resources/logo.png"));
		if (i < 5)
		{
			entities[i]->move((i + 1) * 200.f, 50.f);
			entities[i]->setSpeed(0.f, 0.1f);
		}
		else
		{
			entities[i]->move((i - 5) * 200.f, 550.f);
			entities[i]->setSpeed(0.f, -0.1f);
		}
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		//check collisions
		BF::checkCollisions();

		//update entities
		BF::updateEntities();

		//draw entities
		BF::drawEntities(window);

		window.display();
	}

	return 0;
}