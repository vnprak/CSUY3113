#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"
#include "Map.h"

struct GameState {
	Map* map;
	Entity* player;
	Entity* enemies;
	Entity* projectiles;
	int nextScene;
	int lives = 0;
	int projectileCount = 0;
	int maxBullets = 0;
	bool gameWon = false;
	bool gameLost = false;
};
class Scene {
public:
	GameState state;
	virtual void Initialize() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(ShaderProgram* program) = 0;
	virtual void setLives(int lives) = 0;
	virtual int getLives() = 0;
	virtual bool isGameLost() = 0;
	virtual bool isGameWon() = 0;
	virtual void setProjectiles(int count) = 0;
	virtual int getProjectiles() = 0;
	virtual int projectileMax() = 0;
	virtual int enemyCount() = 0;
};