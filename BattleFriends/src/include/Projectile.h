#pragma once
#include <Entity.h>

namespace BF
{
	class Projectile : public Entity
	{
	public:
		Projectile() = delete;
		Projectile(Projectile&) = delete;
		Projectile(const sf::Vector2f& pos, const sf::Vector2f& speed);
		Projectile(Projectile&& other) noexcept;
		Projectile& operator= (const Projectile&);
		Projectile& operator= (const Projectile&&) noexcept;
		void collide(Entity& other) override;

	private:
		int damage = 10;
	};
}
