#pragma once
#include "Actor.h"
class Spawner : public Actor
{
public:
	Spawner(int x, int y, int color, Actor* target);

	void start() override;
	void update(float deltaTime) override;

private:
	float m_timer = 0;
	float m_spawnDelay = 5;
	MathLibrary::Vector2 m_goalPosition;
	Actor* m_target;
};

