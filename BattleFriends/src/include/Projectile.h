#pragma once
#include <Entity.h>

namespace BF
{
	class Projectile : public Entity
	{
	public:
		Projectile();
		void collide(Entity& other);

	private:
		int damage = 10;
	};
}
