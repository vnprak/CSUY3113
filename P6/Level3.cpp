#include "Level3.h"

#define LEVEL3_WIDTH 21
#define LEVEL3_HEIGHT 8

#define LEVEL3_ENEMY_COUNT 1

unsigned int level3_data[] =
{
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3,
 3, 1, 1, 1, 1, 1, 1, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
 3, 2, 2, 2, 2, 2, 2, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

void Level3::Initialize() {

    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);
    // Move over all of the player and enemy code from initialization.
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(3, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("MechSheet.png");

    state.player->animRight = new int[4]{ 18, 19, 20, 21 };
    state.player->animLeft = new int[4]{ 42, 43, 44, 45 };
    state.player->animUp = new int[4]{ 30, 31, 32, 33 };
    state.player->animDown = new int[4]{ 7, 8, 9, 6 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 12;
    state.player->animRows = 8;

    state.player->height = 0.8f;
    state.player->width = 0.8f;



    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies->position = glm::vec3(14, -3, 0);
    state.enemies[0].speed = 2.5;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].acceleration = glm::vec3(0, -9.81f, 0);
    //state.enemies[0].aiState = IDLE;
    //state.enemies[0].isActive = false;

}
void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, 1, state.map);

    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++)
    {
        state.enemies[i].Update(deltaTime, state.player, state.player, 1, state.map);
    }

    if (state.player->position.x >= 18) {
        state.player->isActive = false;
    }
}
void Level3::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++)
    {
        state.enemies[i].Render(program);
    }
}

void Level3::setLives(int lives)
{
    state.lives = lives;
}

int Level3::getLives()
{
    return state.lives;
}