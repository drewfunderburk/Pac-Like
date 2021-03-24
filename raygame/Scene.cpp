#include "Scene.h"

Scene::Scene()
{
    m_world = new MathLibrary::Matrix3();
}

MathLibrary::Matrix3* Scene::getWorld()
{
    return m_world;
}

void Scene::addActor(Actor* actor)
{
    if (actor == nullptr) return;
    m_actors.push_back(actor);
}

bool Scene::removeActor(int index)
{
    if (index >= 0 && index < m_actors.size()) {
        m_actors.erase(m_actors.begin() + index);
        return true;
    }
    return false;
}

bool Scene::removeActor(Actor* actor)
{
    if (actor == nullptr) return false;
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i] == actor) {
            m_actors.erase(m_actors.begin() + i);
            return true;
        }
    }
    return false;
}

void Scene::start()
{
    m_started = true;
}

void Scene::checkCollision()
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        //Skip this actor if it doesn't collide
        if (m_actors[i]->getStatic())
            continue;
        //Check this actor against each other actor
        for (int j = 0; j < m_actors.size(); j++)
        {
            if (i >= m_actors.size())
                break;

            if (m_actors[i]->checkCollision(m_actors[j]) && i != j)
                m_actors[i]->onCollision(m_actors[j]);
        }
    }
}

void Scene::update(float deltaTime)
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (!m_actors[i]->getStarted())
            m_actors[i]->start();

        m_actors[i]->update(deltaTime);
    }
    checkCollision();
}

void Scene::draw()
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        m_actors[i]->draw();
    }
}

void Scene::debug()
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->getStarted())
            m_actors[i]->debug();
    }
}

void Scene::end()
{
    for (int i = 0; i < m_actors.size(); i++)
    {
        if (m_actors[i]->getStarted())
            m_actors[i]->end();
    }

    m_started = false;
}

