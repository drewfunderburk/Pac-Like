#include "Spawner.h"
#include "Game.h"
#include "Ghost.h"
#include "Maze.h"
#include "raylib.h"

Spawner::Spawner(int x, int y, int color, float spawnDelay, Actor* target) :
	Actor(x, y, 10, 0, (int)0xFF0000FF)
{
	m_spawnDelay = spawnDelay;
	m_target = target;
}

void Spawner::start()
{
	setStatic(true);
}

void Spawner::update(float deltaTime)
{
	// Increment timer
	m_timer += deltaTime;

	// If time between spawns is greater than spawn delay
	if (m_timer > m_spawnDelay)
	{
		// Reset timer
		m_timer = 0;

		// Check if the current scene is a maze
		if (Maze* maze = dynamic_cast<Maze*>(Game::getInstance()->getCurrentScene()))
		{
			MathLibrary::Vector2 position = getWorldPosition();
			// Add a new ghosty boi
			Ghost* enemy = new Ghost(position.x, position.y, 200.0f, 0xFF0000FF, m_target->getWorldPosition(), maze);
			enemy->setTargetPosition(m_target->getWorldPosition());
			enemy->setGoalPosition(m_target->getWorldPosition());
			maze->addActor(enemy);
		}
	}
}
