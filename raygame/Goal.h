#pragma once
#include "Actor.h"
class Goal : public Actor
{
public:
	using Actor::Actor;

	void draw() override;
	void onCollision(Actor* other) override;

private:
	int m_health = 10;
};

