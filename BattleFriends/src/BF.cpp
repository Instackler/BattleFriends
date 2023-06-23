#include <BF.h>

static std::vector<BF::Entity*> game_Entities;
int BF::Entity::num = 0;

BF::Entity::Entity(const char* filename)
	:m_SpeedX(0.f), m_SpeedY(0.f)
{
	m_texture.loadFromFile(filename);
	sf::Sprite::setTexture(m_texture);

	game_Entities.push_back(this);
	num++;

	std::cout << "Created Entity" << std::endl;
}

BF::Entity::Entity()
	:m_SpeedX(0.f), m_SpeedY(0.f)
{
	game_Entities.push_back(this);
	num++;
	std::cout << "Created empty Entity" << std::endl;
}

BF::Entity::~Entity()
{
	for (int i = 0; i < num; i++)
	{
		if (game_Entities[i] == this)
		{
			game_Entities.erase(game_Entities.begin() + i);
		}
	}
	num--;
	game_Entities.shrink_to_fit();
	std::cout << "Destroyed Entity" << std::endl;
}

void BF::Entity::loadTexture(const char* filename)
{
	m_texture.loadFromFile(filename);
}

void BF::Entity::setTexture()
{
	sf::Sprite::setTexture(m_texture);
}

void BF::Entity::setSpeed(float x, float y)
{
	m_SpeedX = x;
	m_SpeedY = y;
}

void BF::Entity::update()
{
	move(m_SpeedX, m_SpeedY);
	m_collided = false;
}


void BF::bounce(Entity& a, Entity& b)
{
	if (a.m_collided || b.m_collided)
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
	b.m_collided = true;
}

void BF::update_Entities()
{
	for (int i = 0; i < game_Entities.size(); i++)
	{
		game_Entities[i]->update();
	}
}

void BF::draw_Entities(sf::RenderTarget& target)
{
	for (int i = 0; i < game_Entities.size(); i++)
	{
		target.draw(*(game_Entities[i]));
	}
}

void BF::check_collisions()
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
{
	loadTexture(filename);
	std::cout << "Created Player" << std::endl;
}

BF::Player::~Player()
{
	std::cout << "Destroyed Player" << std::endl;
}

void BF::Player::update()
{
	if (m_changedDirecton)
	{
		setTexture();
		m_changedDirecton = false;
	}

	Entity::update();
}
