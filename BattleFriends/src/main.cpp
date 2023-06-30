#include <SFML/Graphics.hpp>
#include <BF.h>
#include <iostream>
#include <Windows.h>
#include <winres.h>
#include <resource.h>
#define ENTITY_NUM 9



sf::Texture loadTexture(int name)
{
	HRSRC hResource = FindResourceA(NULL, MAKEINTRESOURCE(name), TEXT("PNG"));
	if (hResource != NULL) {
		DWORD resourceSize = SizeofResource(NULL, hResource);
		HGLOBAL hResourceData = LoadResource(NULL, hResource);
		if (hResourceData != NULL) {

			sf::Texture texture;
			LPVOID pData = LockResource(hResourceData);

			sf::Image image;
			image.loadFromMemory(pData, resourceSize);
			
			texture.create(image.getSize().x, image.getSize().y);
			texture.update(image);

			FreeResource(hResourceData);

			return texture;
		}
		// return errTexture;
	}
	// return noTexture;
}



int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!", sf::Style::Close);
	//window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	
	sf::Sprite spr;
	sf::Texture tex = loadTexture(IDB_PNG1);
	spr.setTexture(tex);
	
	
	


	/*
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
			entities[i]->setSpeed(((rand() % 11) - 5) / 20.f, (rand() % 11) / 10.f);
		}
		else
		{
			entities[i]->move((i - 3) * 210.f, 600.f);
			entities[i]->setSpeed(((rand() % 11) - 5) / 20.f, ((rand() % 11) - 10) / 10.f);
		}
	}
	entities[1]->stationary = true;
	*/

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

		window.draw(spr);

		window.display();
	}

	return 0;
}