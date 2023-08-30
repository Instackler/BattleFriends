#pragma once
#include <Entity.h>

namespace BF
{
	class Projectile : public Entity
	{
	public:
		Projectile() = delete;
		Projectile(const sf::Vector2f& pos, const sf::Vector2f& speed);

		void collide(Entity& other);
		void collide_with_map();

	private:
		int damage = 20;
	};
}
