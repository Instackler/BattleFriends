#pragma once
#include <Entity.h>

namespace BF
{
	class Projectile : public Entity
	{
	public:
		Projectile() = delete;
		Projectile(const Projectile&);
		Projectile(const sf::Vector2f& pos, const sf::Vector2f& speed);
		Projectile(Projectile&& other) noexcept;
		Projectile& operator= (const Projectile&);
		Projectile& operator= (Projectile&&) noexcept;
		void collide(Entity& other);
		void collide_with_map();

	private:
		int damage = 20;
	};
}
