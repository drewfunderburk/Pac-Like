#pragma once
#include "Scene.h"
#include <vector>

struct Camera2D;

class Game
{
public:
    Game();

    static Game* getInstance() { return m_instance; }

    /// <summary>
    /// Returns the scene at the index given.
    /// Returns an empty scene if the index is out of bounds
    /// </summary>
    /// <param name="index">The index of the desired scene</param>
    /// <returns></returns>
    Scene* getScene(int index);


    /// <summary>
    /// Returns the scene that is at the index of the 
    /// current scene index
    /// </summary>
    /// <returns></returns>
    Scene* getCurrentScene();

    /// <returns>The index of the current scene in the game's array of scenes.</returns>
    int getCurrentSceneIndex();

    /// <summary>
    /// Adds the given scene to the array of scenes.
    /// </summary>
    /// <param name="scene">The scene that will be added to the array</param>
    /// <returns>The index the scene was placed at. Returns -1 if
    /// the scene is null</returns>
    int addScene(Scene* scene);

    /// <summary>
    /// Finds the instance of the scene given that inside of the array
    /// and removes it
    /// </summary>
    /// <param name="scene">The scene that will be removed</param>
    /// <returns>If the scene was successfully removed</returns>
    bool removeScene(Scene* scene);


    /// <summary>
    /// Sets the current scene in the game to be the scene at the given index
    /// </summary>
    /// <param name="index">The index of the scene to switch to</param>
    void setCurrentScene(int index);


    /// <summary>
    /// Returns true while a key is being pressed
    /// </summary>
    /// <param name="key">The ascii value of the key to check</param>
    /// <returns></returns>
    bool getKeyDown(int key);

    /// <summary>
    /// Returns true while if key was pressed once
    /// </summary>
    /// <param name="key">The ascii value of the key to check</param>
    /// <returns></returns>
    bool getKeyPressed(int key);

    /// <summary>
    /// Removes the actor from the scene, removes it from its parent, calls its end function, and deletes the actor.
    /// </summary>
    /// <param name="actor">The actor that will be deleted.</param>
    void destroy(Actor* actor);

    /// <summary>
    /// Sets the game state to be the given value. If true, the game will end.
    /// </summary>
    /// <param name="value">The value to set game over.</param>
    void setGameOver(bool value);

    /// <returns>The world matrix of the current scene.</returns>
    MathLibrary::Matrix3* getWorld();

    void run();

private:
	void start();
	void update(float deltaTime);
	void draw();
	void end();

private:
    static Game* m_instance;

    Camera2D* m_camera;
	std::vector<Scene*> m_scenes;
    bool m_gameOver = false;
    int m_currentSceneIndex;
};
