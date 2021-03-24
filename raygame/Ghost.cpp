#include "Ghost.h"
#include "Maze.h"
#include "Wall.h"
#include "raylib.h"

Ghost::Ghost(float x, float y, float maxSpeed, int color, Maze* maze)
	: Agent(x, y, Maze::TILE_SIZE / 2.5f, maxSpeed, maxSpeed, color)
{
	m_maze = maze;
	m_pathfindBehavior = new SeekPathBehavior(maze);
	m_pathfindBehavior->setColor(color);
	addBehavior(m_pathfindBehavior);
}

Ghost::~Ghost()
{
	delete m_pathfindBehavior;
}

void Ghost::update(float deltaTime)
{
	Agent::update(deltaTime);
}

void Ghost::draw()
{
	m_pathfindBehavior->draw(this);
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
}

void Ghost::setTarget(Actor* target)
{
	m_target = target;
	m_pathfindBehavior->setTarget(target);
}

Actor* Ghost::getTarget()
{
	return m_target;
}
