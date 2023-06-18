#include <SFML/Graphics.hpp>
#include <BF.h>


int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!", sf::Style::Close);
	//window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	//spawn entitites
	std::vector<BF::Entity> entities;
	BF::Entity* e;
	for (int i = 0; i < 10; i++)
	{
		e = new BF::Entity("resources/logo.png");
		entities.push_back(*e);
		entities[i].move(i * 200.f, i * 100.f);
	}
	
	//set random speed for entities
	srand(time(0));
	int rx = 0, ry = 0;
	for (int i = 0; i < 10; i++)
	{
		rx = (rand() % 11) - 5;
		ry = (rand() % 11) - 5;
		entities[i].setSpeed(rx / 10.f, ry / 10.f);
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
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (j != i)
				{
					if (entities[i].getGlobalBounds().intersects(entities[j].getGlobalBounds()))
					{
						entities[i].bounce();
					}
				}
			}
		}

		//update entities
		for (int i = 0; i < 10; i++)
		{
			entities[i].update();
		}

		//draw entities
		for (int i = 0; i < 10; i++)
		{
			window.draw(entities[i]);
		}

		window.display();
	}

	return 0;
}