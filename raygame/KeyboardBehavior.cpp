#include "KeyboardBehavior.h"
#include "raylib.h"
#include <Vector2.h>

void KeyboardBehavior::update(Agent* owner, float deltaTime)
{
    MathLibrary::Vector2 direction = MathLibrary::Vector2();

    owner->setVelocity({ 0, 0 });
    if (IsKeyDown(KEY_W))
        direction.y = -1;
    if (IsKeyDown(KEY_S))
        direction.y = 1;
    if (IsKeyDown(KEY_A))
        direction.x = -1;
    if (IsKeyDown(KEY_D))
        direction.x = 1;

    owner->setVelocity(direction * m_speedIncrement);
}
