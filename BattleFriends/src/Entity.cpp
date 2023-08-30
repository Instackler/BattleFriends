#include <pch.h>
#include <Entity.h>
#include <BF.h>


BF::Entity::Entity(const std::string& textureID)
	:m_textureID(textureID)
{
	setTexture(textures[textureID]);

	sf::FloatRect bounding_box = getLocalBounds();
	setOrigin(bounding_box.width / 2.f, bounding_box.height / 2.f);
	radius = bounding_box.width < bounding_box.height ?
		bounding_box.width / 2.f : bounding_box.height / 2.f;

	// TODO: add logging		std::cout << "Created Entity" << std::endl;
}

void BF::Entity::setSpeed(float x, float y)
{
	m_SpeedX = x;
	m_SpeedY = y;
}

void BF::Entity::update()
{
	if (stationary)
	{
		setSpeed(0.f, 0.f);
	}
	else
	{
		move(m_SpeedX, m_SpeedY);
	}
}

bool BF::Entity::intersects(const Entity& other)
{
	float x = getPosition().x;
	float y = getPosition().y;

	float other_x = other.getPosition().x;
	float other_y = other.getPosition().y;

	if (std::hypotf(std::abs(x - other_x), std::abs(y - other_y)) < radius + other.radius)
	{
		return true;
	}

	return false;
}

bool BF::Entity::intersects_map(const Entity& other)
{
	sf::Vector2f nearest{};
	float x_min = other.getGlobalBounds().left;
	float x_max = x_min + other.getGlobalBounds().width;
	float y_min = other.getGlobalBounds().top;
	float y_max = y_min + other.getGlobalBounds().height;
	nearest.x = std::clamp(getPosition().x, x_min, x_max);
	nearest.y = std::clamp(getPosition().y, y_min, y_max);
	sf::Vector2f diff{ nearest.x - getPosition().x, nearest.y - getPosition().y };
	float overlap = radius - std::hypotf(std::abs(diff.x), std::abs(diff.y));

	if (overlap > 0.f)
	{
		return true;
	}
	return false;
}

bool BF::Entity::is_dead()
{
	return health > 0 ? false : true;
}

bool BF::Entity::out_of_bounds(sf::FloatRect area)
{
	sf::Vector2f pos = getPosition();
	return !area.contains(pos.x + radius / 2, pos.y + radius / 2);
}

void BF::Entity::set_points()
{
	float rot = getRotation();
	setRotation(0.f);
	sf::FloatRect box = getGlobalBounds();
	points[0] = { box.left, box.top };
	points[1] = { box.left, box.top + box.height };
	points[2] = { box.left + box.width, box.top + box.height };
	points[3] = { box.left + box.width, box.top };
	setRotation(rot);
}
