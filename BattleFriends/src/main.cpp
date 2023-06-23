#include <SFML/Graphics.hpp>
#include <BF.h>
#include <iostream>



int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!", sf::Style::Close);
	//window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	
	BF::Entity player1("resources/logo.png");
	BF::Player player2("resources/logo.png");
	player1.setSpeed(0.f, 1.f);
	player2.move(0.f, 400.f);
	player2.setSpeed(0.f, -1.f);
	
	/*std::vector<bf::Entity> entities;
	for (int i = 0; i < 10; i++)
	{
		entities.emplace_back(bf::Entity("resources/logo.png"));
		if (i < 5)
		{
			entities[i].move(0.f, i * 100.f);
		}
		else
		{
			entities[i].move(500.f, i * 100.f);
		}
	}*/

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
		BF::check_collisions();

		//update entities
		BF::update_Entities();

		//draw entities
		BF::draw_Entities(window);

		window.display();
	}


	return 0;
}