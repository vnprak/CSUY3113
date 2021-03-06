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
#include "Map.h"

enum EntityType{PLAYER, PLATFORM, ENEMY, PROJECTILE};

enum AIType{ WALKER, WAITANDGO };
enum AIState{ IDLE, WALKING, ATTACKING };

enum Direction{ UP, DOWN, LEFT, RIGHT };

class Entity {
public:
    EntityType entityType;
    AIType aiType;
    AIState aiState;
    Direction direction;

    glm::vec3 position;
    glm::vec3 spawn;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;

    float width = 1;
    float height = 1;
    float colwidth = 1;
    float colheight = 1;

    float speed;
    
    GLuint textureID;
    
    glm::mat4 modelMatrix;
    
    int *animRight = NULL;
    int *animLeft = NULL;
    int *animUp = NULL;
    int *animDown = NULL;

    int* shootRight = NULL;
    int* shootLeft = NULL;
    int* shootUp = NULL;
    int* shootDown = NULL;


    int *animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;

    bool isActive = true;

    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    bool hit = false;
    int invuln = 0;

    Entity* owner = NULL;
    
    Entity();
    

    bool CheckCollision(Entity* other);
    void CheckCollisionsY(Entity* objects, int objectCount);
    void CheckCollisionsX(Entity* objects, int objectCount);
    void CheckCollisionsX(Map* map);
    void CheckCollisionsY(Map* map);

    void Update(float deltaTime, Entity *player, Entity* objects, int objectCount, Map *map);
    void Render(ShaderProgram *program);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);

    void AI(Entity *player);
    void AIWalker();
    void AIWaitAndGo(Entity *player);
    bool entityHit();
    void fire(Entity* projectile);
    void flight();
};
