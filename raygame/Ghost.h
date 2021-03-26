#pragma once
#include "Agent.h"
#include "SeekPathBehavior.h"
class Maze;
class SeePlayerDecision;

class Ghost : public Agent
{
public:
	/// <param name="x">The x-position</param>
	/// <param name="y">The y-position</param>
	/// <param name="speed">The maximum movement speed (magnitude)</param>
	/// <param name="color">The color in 0xRRGGBBAA</param>
	/// <param name="maze">The maze used for pathfinding</param>
	Ghost(float x, float y, float speed, int color, MathLibrary::Vector2 goalPosition, Maze* maze);
	~Ghost();

	virtual void update(float deltaTime) override;
	virtual void draw() override;

	virtual void onCollision(Actor* other) override;

	/// <returns>The current target</returns>
	MathLibrary::Vector2 getTargetPosition();
	/// <summary>
	/// Set the target of the ghost
	/// </summary>
	/// <param name="target">The new target</param>
	void setTargetPosition(MathLibrary::Vector2 target);

	/// <summary>
	/// Get goal position
	/// </summary>
	MathLibrary::Vector2 getGoalPosition() { return m_goalPosition; }

	/// <summary>
	/// Set the goal position
	/// </summary>
	void setGoalPosition(MathLibrary::Vector2 position) { m_goalPosition = position; }

	/// <summary>
	/// Get the maze
	/// </summary>
	/// <returns></returns>
	MathLibrary::Vector2 getPlayerPosition();
private:
	SeekPathBehavior* m_pathfindBehavior;
	SeePlayerDecision* m_seePlayerDecision;
	Maze* m_maze;
	MathLibrary::Vector2 m_goalPosition;
	MathLibrary::Vector2 m_targetPosition;
};

