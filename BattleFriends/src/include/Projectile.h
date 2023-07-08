#pragma once
#include <Entity.h>

namespace BF
{
	class Projectile : public Entity
	{
	public:
		Projectile();
		void collide(Entity& other) override;

	private:
		int damage = 10;
	};
}
