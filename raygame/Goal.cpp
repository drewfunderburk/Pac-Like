#include "Goal.h"
#include "Ghost.h"
#include "Game.h"
#include "raylib.h"

void Goal::draw()
{
	Actor::draw();

	char buffer[10];
	sprintf_s(buffer, "%d", m_health);
	DrawText(buffer, (int)getWorldPosition().x, (int)getWorldPosition().y, 24, RAYWHITE);
}

void Goal::onCollision(Actor* other)
{
	// If the collided actor was an enemy
	if (Ghost* enemy = dynamic_cast<Ghost*>(other))
	{
		// Destroy enemy
		Game::getInstance()->destroy(enemy);

		// Decrement health
		m_health--;
		if (m_health <= 0)
			Game::getInstance()->setGameOver(true);
	}
}
