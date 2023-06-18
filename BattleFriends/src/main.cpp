#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!", sf::Style::Close);
	//window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	// create an array of 3 vertices that define a triangle primitive
	sf::VertexArray triangle(sf::Triangles, 3);

	// define the position of the triangle's points
	triangle[0].position = sf::Vector2f(10.f, 10.f);
	triangle[1].position = sf::Vector2f(100.f, 10.f);
	triangle[2].position = sf::Vector2f(100.f, 100.f);

	// define the color of the triangle's points
	triangle[0].color = sf::Color::Red;
	triangle[1].color = sf::Color::Blue;
	triangle[2].color = sf::Color::Green;

	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(triangle);
		window.display();
	}

	return 0;
}