#pragma once
#include <Entity.h>


namespace BF 
{
	class Player : public Entity
	{
	public:
		Player(const char* filename);
		~Player();

		void update() override;

	private:
		void checkInput();
		void shoot();

	private:
	};
}