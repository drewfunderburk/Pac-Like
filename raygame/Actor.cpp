#include <cmath>
#include "Actor.h"
#include "raylib.h"
#include "Sprite.h"

Actor::Actor()
{
    m_globalTransform = new MathLibrary::Matrix3();
    m_localTransform = new MathLibrary::Matrix3();

    m_rotation = new MathLibrary::Matrix3();
    m_translation = new MathLibrary::Matrix3();
    m_scale = new MathLibrary::Matrix3();

    m_velocity = MathLibrary::Vector2();
    m_acceleration = MathLibrary::Vector2();
    m_collisionRadius = 0;
    m_maxSpeed = 1;
    setLocalPosition(MathLibrary::Vector2(0, 0));

    m_icon = ' ';
    m_color = 0x00000000;
}

Actor::Actor(float x, float y, float collisionRadius, float maxSpeed, char icon = ' ')
    : Actor()
{
    // Set the position and update transforms
    setLocalPosition(MathLibrary::Vector2(x, y));
    *m_localTransform = *m_translation * *m_rotation * *m_scale;
    updateGlobalTransform();

    m_collisionRadius = collisionRadius;
    m_maxSpeed = maxSpeed;
    m_icon = icon;
    m_color = 0xFFFFFFFF;
}

Actor::Actor(float x, float y, float collisionRadius, float maxSpeed, int color)
    : Actor(x, y, collisionRadius, maxSpeed, ' ')
{
    m_color = color;
}

Actor::Actor(float x, float y, float collisionRadius, float maxSpeed, Sprite* sprite)
    : Actor(x, y, collisionRadius, maxSpeed, ' ')
{
    m_sprite = sprite;
}

Actor::Actor(float x, float y, float collisionRadius, float maxSpeed, const char* spriteFilePath)
    : Actor(x, y, collisionRadius, maxSpeed, ' ')
{
    m_sprite = new Sprite(spriteFilePath);
}

MathLibrary::Vector2 Actor::getForward()
{
    return MathLibrary::Vector2(m_globalTransform->m11, m_globalTransform->m21).getNormalized();
}

void Actor::setForward(MathLibrary::Vector2 value)
{
    MathLibrary::Vector2 lookPosition = getWorldPosition() + value.getNormalized();
    lookAt(lookPosition);
}

MathLibrary::Vector2 Actor::getWorldPosition()
{
    return MathLibrary::Vector2(m_globalTransform->m13, m_globalTransform->m23);
}

void Actor::setWorldPostion(MathLibrary::Vector2 value)
{
    if (m_parent)
    {
        setLocalPosition(value + m_parent->getWorldPosition());
    }
    else
    {
        setLocalPosition(value);
    }
}

MathLibrary::Vector2 Actor::getLocalPosition()
{
    return MathLibrary::Vector2(m_localTransform->m13, m_localTransform->m23);
}

void Actor::setLocalPosition(MathLibrary::Vector2 value)
{
    *m_translation = MathLibrary::Matrix3::createTranslation(value);
}

void Actor::start()
{
    m_started = true;
}

void Actor::addChild(Actor* child)
{
    m_children.push_back(child);
}

void Actor::removeChild(int index)
{
    m_children.erase(m_children.begin() + index);
}

void Actor::removeChild(Actor* child)
{
    for (int i = 0; i < m_children.size(); i++)
    {
        if (m_children[i] = child) {
            m_children.erase(m_children.begin() + i);
            break;
        }
    }
}

void Actor::setScale(MathLibrary::Vector2 scale)
{
    *m_scale = MathLibrary::Matrix3::createScale(scale);
}

void Actor::scale(MathLibrary::Vector2 scale)
{
    *m_scale = *m_scale * MathLibrary::Matrix3::createScale(scale);
}

void Actor::setRotation(float radians)
{
    *m_rotation = MathLibrary::Matrix3::createRotation(radians);
}

void Actor::rotate(float radians)
{
    *m_rotation = *m_rotation * MathLibrary::Matrix3::createRotation(radians);
}

void Actor::lookAt(MathLibrary::Vector2 position)
{
    //Find the direction that the actor should look in
    MathLibrary::Vector2 direction = (position - getWorldPosition()).getNormalized();
    
    //Use the dotproduct to find the angle the actor needs to rotate
    float dotProd = MathLibrary::Vector2::dotProduct(getForward(), direction);
    if (abs(dotProd) > 1)
        return;
    float angle = (float)acos(dotProd);

    //Find a perpindicular vector to the direction
    MathLibrary::Vector2 perp = MathLibrary::Vector2(direction.y, -direction.x);

    //Find the dot product of the perpindicular vector and the current forward
    float perpDot = MathLibrary::Vector2::dotProduct(perp, getForward());

    //If the result isn't 0, use it to change the sign of the angle to be either positive or negative
    if (perpDot != 0)
        angle *= -perpDot / abs(perpDot);

    rotate(angle);
}

bool Actor::checkCollision(Actor* other)
{
    if (!other)
        return false;

    float distance = (other->getWorldPosition() - getWorldPosition()).getMagnitude();
    return distance <= m_collisionRadius + other->m_collisionRadius;
}

void Actor::onCollision(Actor* other)
{
}

void Actor::update(float deltaTime)
{
    *m_localTransform = *m_translation * *m_rotation * *m_scale;

    updateGlobalTransform();

    setVelocity(m_velocity + m_acceleration);

    if (m_velocity.getMagnitude() > m_maxSpeed)
        m_velocity = m_velocity.getNormalized() * m_maxSpeed;

    //Increase position by the current velocity
    setLocalPosition(getLocalPosition() + m_velocity * deltaTime);
}

void Actor::draw()
{
    DrawCircle((int)getWorldPosition().x, (int)getWorldPosition().y, m_collisionRadius, GetColor(m_color));
    //Draws the actor and a line indicating it facing to the raylib window
    DrawLine(
        (int)(getWorldPosition().x),
        (int)(getWorldPosition().y),
        (int)((getWorldPosition().x + getForward().x)),
        (int)((getWorldPosition().y + getForward().y)),
        WHITE
    );

    if (m_sprite)
        m_sprite->draw(*m_globalTransform);
}

void Actor::debug()
{
}


void Actor::end()
{
    m_started = false;
}

void Actor::updateFacing()
{
    if (m_velocity.getMagnitude() <= 0)
        return;

    setForward(m_velocity.getNormalized());
}

void Actor::updateGlobalTransform()
{
    if (m_parent)
        *m_globalTransform = *(m_parent->m_globalTransform) * (*m_localTransform);
    else
        *m_globalTransform = *m_localTransform;

    for (int i = 0; i < m_children.size(); i++)
    {
        m_children[i]->updateGlobalTransform();
    }
}
