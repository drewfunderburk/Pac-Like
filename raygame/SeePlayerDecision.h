#pragma once
#include "Behavior.h"
class SeekPathBehavior;

class SeePlayerDecision : public Behavior
{
	enum State
	{
		PATH_TO_GOAL,
		EVADE_PLAYER
	};

public:
	SeePlayerDecision(SeekPathBehavior* pathBehavior, MathLibrary::Vector2 goalPosition, float visionDistance);

	void update(Agent* owner, float deltaTime) override;

private:
	/// <summary>
	/// Returns whether the player can be seen based on its given vision distance
	/// </summary>
	/// <param name="owner"></param>
	/// <returns></returns>
	bool canSeePlayer(Agent* owner);

	/// <summary>
	/// Returns the best adjacent corner to flee to based on the player's location
	/// </summary>
	/// <param name="owner"></param>
	/// <returns></returns>
	MathLibrary::Vector2 findCorner(Agent* owner);

private:
	SeekPathBehavior* m_seekPathBehavior;
	MathLibrary::Vector2 m_goalPosition;
	float m_visionDistance;
	State m_currentState = PATH_TO_GOAL;
};

