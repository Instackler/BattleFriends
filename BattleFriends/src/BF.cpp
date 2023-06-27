#include <BF.h>

static std::vector<BF::Entity*> game_Entities;
static bool init = true;
int BF::Entity::s_num = 0;

BF::Entity::Entity(const char* filename)
{
	m_texture.loadFromFile(filename);
	sf::Sprite::setTexture(m_texture);

	sf::FloatRect bounding_box = getLocalBounds();
	setOrigin(bounding_box.width / 2.f, bounding_box.height / 2.f);

	game_Entities.push_back(this);
	s_num++;

	std::cout << "Created Entity" << std::endl;
}

BF::Entity::~Entity()
{
	for (int i = 0; i < s_num; i++)
	{
		if (game_Entities[i] == this)
		{
			game_Entities.erase(game_Entities.begin() + i);
			s_num--;
		}
	}
	game_Entities.shrink_to_fit();
	std::cout << "Destroyed Entity" << std::endl;
}

BF::Entity::Entity(Entity&& other) noexcept
{
	m_texture = other.m_texture;
	std::cout << "Copied Entity" << std::endl;
}

void BF::Entity::setSpeed(float x, float y)
{
	m_SpeedX = x;
	m_SpeedY = y;
}

void BF::Entity::update()
{
	move(m_SpeedX, m_SpeedY);
}

bool BF::Entity::intersects(const Entity& other)
{


	return false;
}


void BF::bounce(Entity& a, Entity& b)
{
	/*if (a.m_collided || b.m_collided)
	{
		return;
	}
	else
	{
		float a_prevSpeedX = a.m_SpeedX;
		a.m_SpeedX = b.m_SpeedX;
		b.m_SpeedX = a_prevSpeedX;

		float a_prevSpeedY = a.m_SpeedY;
		a.m_SpeedY = b.m_SpeedY;
		b.m_SpeedY = a_prevSpeedY;
	}
	a.m_collided = true;
	b.m_collided = true;*/
}

void BF::updateEntities()
{
	for (int i = 0; i < game_Entities.size(); i++)
	{
		game_Entities[i]->update();
	}
}

void BF::drawEntities(sf::RenderTarget& target)
{
	for (int i = 0; i < game_Entities.size(); i++)
	{
		target.draw(*(game_Entities[i]));
	}
}

void BF::checkCollisions()
{
	for (int i = 0; i < game_Entities.size(); i++)
	{
		for (int j = 0; j < game_Entities.size(); j++)
		{
			if (i != j)
			{
				if (game_Entities[i]->getGlobalBounds().intersects(game_Entities[j]->getGlobalBounds()))
					bounce(*game_Entities[i], *game_Entities[j]);
			}
		}
	}
}


BF::Player::Player(const char* filename)
	:Entity(filename)
{
	std::cout << "Created Player" << std::endl;
}

BF::Player::~Player()
{
	std::cout << "Destroyed Player" << std::endl;
}

void BF::Player::update()
{
	setSpeed(0.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		setSpeed(1.f, 0.f);
	}
	Entity::update();
}
