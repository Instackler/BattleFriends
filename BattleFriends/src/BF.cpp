#include <BF.h>

static std::vector<BF::Entity*> game_Entities;
static bool init = true;
int BF::Entity::s_num = 0;

BF::Entity::Entity(const char* filename)
{
	m_texture.loadFromFile(filename);
	setTexture(m_texture);

	sf::FloatRect bounding_box = getLocalBounds();
	setOrigin(bounding_box.width / 2.f, bounding_box.height / 2.f);
	radius = bounding_box.width < bounding_box.height ?
		bounding_box.width / 2.f : bounding_box.height / 2.f;

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
	float x = getPosition().x;
	float y = getPosition().y;

	float other_x = other.getPosition().x;
	float other_y = other.getPosition().y;

	if (hypotf(fabsf(x - other_x), fabsf(y - other_y)) < radius + other.radius)
	{
		return true;
	}

	return false;
}

void BF::Entity::collide(Entity& other)
{
	if (this->m_collided && other.m_collided)
	{
		return;
	}
	else
	{
		if (this->stationary)
		{
			setSpeed(other.m_SpeedX * -1.f, other.m_SpeedY * -1.f);
		}

		if (other.stationary)
		{
			other.setSpeed(this->m_SpeedX * -1.f, this->m_SpeedY * -1.f);
		}
		
		float dx = other.getPosition().x - this->getPosition().x;
		float dy = other.getPosition().y - this->getPosition().y;
		float d = hypotf(fabsf(dx), fabsf(dy));
		float nx = dx / d;
		float ny = dy / d;
		float p = 2 * (this->m_SpeedX * nx + this->m_SpeedY * ny - other.m_SpeedX * nx - other.m_SpeedY * ny) / (this->radius + other.radius);
		this->m_SpeedX -= p * other.radius * nx;
		this->m_SpeedY -= p * other.radius * ny;
		other.m_SpeedX += p * this->radius * nx;
		other.m_SpeedY += p * this->radius * ny;
	}
	this->m_collided = true;
	other.m_collided = true;
}

void BF::updateEntities()
{
	for (int i = 0; i < game_Entities.size(); i++)
	{
		if (game_Entities[i]->health > 0)
		{
			if (game_Entities[i]->stationary)
			{
				game_Entities[i]->setSpeed(0.f, 0.f);
			}

			game_Entities[i]->update();
			game_Entities[i]->m_collided = false;
		}
	}
}

void BF::drawEntities(sf::RenderTarget& target)
{
	for (int i = 0; i < game_Entities.size(); i++)
	{
		if (game_Entities[i]->health > 0)
		{
			target.draw(*(game_Entities[i]));
		}
	}
}

void BF::checkCollisions()
{
	for (int i = 0; i < game_Entities.size(); i++)
	{
		if (game_Entities[i]->health > 0)
		{
			for (int j = 0; j < game_Entities.size(); j++)
			{
				if ((game_Entities[j]->health > 0) && i != j)
				{
					if (game_Entities[i]->intersects(*game_Entities[j]))
					{
						game_Entities[i]->collide(*game_Entities[j]);
					}
				}
			}
		}
	}
}

void BF::checkHits()
{
	 
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
	if (!m_collided)
	{
		setSpeed(0.f, 0.f);
		checkInput();
	}
	Entity::update();
}

void BF::Player::checkInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		m_SpeedY -= 1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_SpeedX -= 1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_SpeedY += 1.f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_SpeedX += 1.f;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		shoot();
	}
}

void BF::Player::shoot()
{
	
}

BF::Projectile::Projectile()
	:Entity("resources/proj.png")
{
	health = 1;
}

void BF::Projectile::collide(Entity& other)
{
	other.health -= damage;
	this->health--;
}
