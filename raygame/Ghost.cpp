#include "Ghost.h"
#include "Pac.h"
#include "Maze.h"
#include "Wall.h"
#include "SeePlayerDecision.h"
#include "Game.h"
#include "raylib.h"

Ghost::Ghost(float x, float y, float maxSpeed, int color, MathLibrary::Vector2 goalPosition, Maze* maze)
	: Agent(x, y, Maze::TILE_SIZE / 2.5f - 1, maxSpeed, maxSpeed, color)
{
	m_maze = maze;
	m_goalPosition = goalPosition;
	m_pathfindBehavior = new SeekPathBehavior(maze);
	m_seePlayerDecision = new SeePlayerDecision(m_pathfindBehavior,m_goalPosition, 100);
	m_pathfindBehavior->setColor(color);
	addBehavior(m_pathfindBehavior);
	addBehavior(m_seePlayerDecision);
}

Ghost::~Ghost()
{
	delete m_pathfindBehavior;
	delete m_seePlayerDecision;
}

void Ghost::update(float deltaTime)
{
	Agent::update(deltaTime);
}

void Ghost::draw()
{
	//m_pathfindBehavior->draw(this);
	Agent::draw();
}

void Ghost::onCollision(Actor* other)
{
	if (Wall* wall = dynamic_cast<Wall*>(other)) {
		MathLibrary::Vector2 halfTile = { Maze::TILE_SIZE / 2.0f, Maze::TILE_SIZE / 2.0f };
		MathLibrary::Vector2 position = getWorldPosition();
		position = position + halfTile;
		MathLibrary::Vector2 tilePosition = {
			roundf(position.x / Maze::TILE_SIZE) * Maze::TILE_SIZE,
			roundf(position.y / Maze::TILE_SIZE) * Maze::TILE_SIZE
		};
		tilePosition = tilePosition - halfTile;
		setWorldPostion(tilePosition);

		setVelocity({ 0, 0 });
	}

	if (Pac* pac = dynamic_cast<Pac*>(other))
	{
		Game::getInstance()->destroy(this);
	}
}

void Ghost::setTargetPosition(MathLibrary::Vector2 target)
{
	m_targetPosition = target;
	m_pathfindBehavior->setTarget(target);
}

MathLibrary::Vector2 Ghost::getPlayerPosition()
{
	return m_maze->getPlayerPosition();
}

MathLibrary::Vector2 Ghost::getTargetPosition()
{
	return m_targetPosition;
}
