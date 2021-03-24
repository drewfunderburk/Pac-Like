#include "Goal.h"
#include "Ghost.h"
#include "Game.h"

void Goal::onCollision(Actor* other)
{
	// If the collided actor was an enemy
	if (Ghost* enemy = dynamic_cast<Ghost*>(other))
	{
		// Destroy enemy
		Game::getInstance()->destroy(enemy);

		// Decrement health
		m_health--;
	}
}
