#include "Pac.h"
#include "Maze.h"
#include "Wall.h"
#include "raylib.h"

Pac::Pac(float x, float y, float maxSpeed)
	: Agent(x, y, Maze::TILE_SIZE / 2.5f, maxSpeed, maxSpeed, (int)0xFFFF66FF)
{
	m_keyboardBehavior = new KeyboardBehavior(maxSpeed * 100);
	addBehavior(m_keyboardBehavior);
}

Pac::~Pac()
{
	delete m_keyboardBehavior;
}

void Pac::draw()
{
	Agent::draw();
}

void Pac::onCollision(Actor* other)
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
