#include "SeePlayerDecision.h"
#include "SeekPathBehavior.h"
#include "Ghost.h"
#include "raylib.h"
#include <iostream>

SeePlayerDecision::SeePlayerDecision(SeekPathBehavior* pathBehavior, MathLibrary::Vector2 goalPosition, float visionDistance)
{
	m_seekPathBehavior = pathBehavior;
	m_goalPosition = goalPosition;
	m_visionDistance = visionDistance;
}

void SeePlayerDecision::update(Agent* owner, float deltaTime)
{
	// Cast owner to Ghost
	Ghost* ghost = dynamic_cast<Ghost*>(owner);
	if (!ghost)
		return;

	// FSM
	switch (m_currentState)
	{
	case PATH_TO_GOAL:
		// Check if player is in range
		if (canSeePlayer(owner))
		{
			// Set destination to a corner to evade
			ghost->setTargetPosition(findCorner(owner));
			// Update path
			m_seekPathBehavior->updatePath(owner);
			// Transition to EVADE_PLAYER
			m_currentState = EVADE_PLAYER;
		}
		break;
	case EVADE_PLAYER:
		// Check if the player is in range
		if (!canSeePlayer(owner))
		{
			// Set destination to the goal
			m_seekPathBehavior->setTarget(m_goalPosition);
			// Update path
			m_seekPathBehavior->updatePath(owner);
			// Transition to PATH_TO_GOAL
			m_currentState = PATH_TO_GOAL;
		}
		break;
	}
}

bool SeePlayerDecision::canSeePlayer(Agent* owner)
{
	// Cast owner to Ghost*
	Ghost* ghost = dynamic_cast<Ghost*>(owner);
	if (!ghost)
		return false;

	// If the distance between the player and the ghost is less than m_visionDistance return true
	if ((ghost->getPlayerPosition() - ghost->getWorldPosition()).getMagnitude() < m_visionDistance)
		return true;
	return false;
}

MathLibrary::Vector2 SeePlayerDecision::findCorner(Agent* owner)
{
	// Cast owner as Ghost*
	Ghost* ghost = dynamic_cast<Ghost*>(owner);
	if (!ghost)
		return MathLibrary::Vector2();

	// Declare corners
	MathLibrary::Vector2 corners[4] =
	{
		MathLibrary::Vector2(0, 0),
		MathLibrary::Vector2(RAYLIB_H::GetScreenWidth(), 0),
		MathLibrary::Vector2(RAYLIB_H::GetScreenWidth(), RAYLIB_H::GetScreenHeight()),
		MathLibrary::Vector2(0, RAYLIB_H::GetScreenHeight())
	};

	// Find the closest corner
	int closestCorner;
	float closestDistance = -1;
	for (int i = 0; i < 4; i++)
	{
		// Get the distance between owner and a corner
		float magnitude = (owner->getWorldPosition() - corners[i]).getMagnitude();
		// If the closest distance is less than 0, we haven't found one yet so it is the current one
		// If the magnitude found is less than the closest distance, we've found a new closest
		if (closestDistance < 0 || magnitude < closestDistance)
		{
			// Update which corner is the closest
			closestCorner = i;
			// Update closestDistance
			closestDistance = magnitude;
		}
	}

	// Create array to hold adjacent corners
	// If closestCorner - 1 < 0 then wrap around to 3
	// If closestCorner + 1 > 3 then wrap around to 0
	int adjacentCorners[2] = {
		closestCorner - 1 < 0 ? 3 : closestCorner - 1,
		(closestCorner + 1) % 4 };

	// Store player position
	MathLibrary::Vector2 playerPosition = ghost->getPlayerPosition();

	// Store the direction from the enemy to the player
	MathLibrary::Vector2 directionToPlayer = (owner->getWorldPosition() - playerPosition).getNormalized();

	// Store the direction from the enemy to the first adjacent corner
	MathLibrary::Vector2 directionToCorner0 = (owner->getWorldPosition() - corners[adjacentCorners[0]]).getNormalized();

	// Store the direction from the enemy to the second adjacent corner
	MathLibrary::Vector2 directionToCorner1 = (owner->getWorldPosition() - corners[adjacentCorners[1]]).getNormalized();

	// Store the dotproduct of the direction to the player and the direction to the first corner
	float dotProduct0 = MathLibrary::Vector2::dotProduct(directionToPlayer, directionToCorner0);

	// Store the dotproduct of the direction to the player and the direction to the second corner
	float dotProduct1 = MathLibrary::Vector2::dotProduct(directionToPlayer, directionToCorner1);

	// Compare the two dotproducts. 
	// If the first is less than the second, then there is more room to run in that direction.
	// Flee towards that corner
	if (dotProduct0 < dotProduct1)
		return corners[adjacentCorners[0]];
	else
		return corners[adjacentCorners[1]];
}
