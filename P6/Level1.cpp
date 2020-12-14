#include "Level1.h"

#define LEVEL1_WIDTH 15
#define LEVEL1_HEIGHT 15

#define LEVEL1_ENEMY_COUNT 1
#define PROJECTILE_LIMIT 10

unsigned int level1_data[] =
{
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

void Level1::Initialize() {

    state.nextScene = -1;

	GLuint mapTextureID = Util::LoadTexture("tileset.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
	// Move over all of the player and enemy code from initialization.
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, -3, 0);
    state.player->movement = glm::vec3(0);
    state.player->speed = 3.5f;
    state.player->textureID = Util::LoadTexture("MechSheet.png");

    state.player->animRight = new int[4]{ 18, 19, 20, 21 };
    state.player->animLeft = new int[4]{ 42, 43, 44, 45 };
    state.player->animUp = new int[4]{ 30, 31, 32, 33 };
    state.player->animDown = new int[4]{ 7, 8, 9, 6 };

    state.player->shootRight = new int[4]{ 15, 14, 13, 13 };
    state.player->shootLeft = new int[4]{ 39, 38, 37, 37 };
    state.player->shootUp = new int[4]{ 26, 25, 24, 24 };
    state.player->shootDown = new int[4]{ 3, 2, 1, 1 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 12;
    state.player->animRows = 8;

    state.player->height = 2.0f;
    state.player->width = 2.0f;
    state.player->colheight = 1.2f;
    state.player->colwidth = 1.4f;

    state.projectiles = new Entity[PROJECTILE_LIMIT];
    GLuint projectileTextureID = Util::LoadTexture("ball.png");

    for (int i = 0; i < PROJECTILE_LIMIT; i++)
    {
        state.projectiles[i].entityType = PROJECTILE;
        state.projectiles[i].textureID = projectileTextureID;
        state.projectiles[i].position = glm::vec3(0);
        state.projectiles[i].speed = 5;
        state.projectiles[i].owner = NULL;
        state.projectiles[i].isActive = false;

        state.projectiles[i].animRight = new int[4]{0, 3, 6, 3};

        state.projectiles[0].animIndices = state.projectiles[0].animRight;
        state.projectiles[0].animFrames = 4;
        state.projectiles[0].animIndex = 0;
        state.projectiles[0].animTime = 0;
        state.projectiles[0].animCols = 3;
        state.projectiles[0].animRows = 4;
    }

    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("MechSheet Enemy.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies->position = glm::vec3(15, -3, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;

    state.enemies[0].animRight = new int[4]{ 18, 19, 20, 21 };
    state.enemies[0].animLeft = new int[4]{ 42, 43, 44, 45 };
    state.enemies[0].animUp = new int[4]{ 30, 31, 32, 33 };
    state.enemies[0].animDown = new int[4]{ 7, 8, 9, 6 };

    state.enemies[0].animIndices = state.enemies[0].animRight;
    state.enemies[0].animFrames = 4;
    state.enemies[0].animIndex = 0;
    state.enemies[0].animTime = 0;
    state.enemies[0].animCols = 12;
    state.enemies[0].animRows = 8;

    state.enemies[0].height = 2.0f;
    state.enemies[0].width = 2.0f;
    state.enemies[0].colheight = 1.2f;
    state.enemies[0].colwidth = 1.4f;

}
void Level1::Update(float deltaTime) {
    state.player->hit = false;
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) state.enemies[i].hit = false;
	state.player->Update(deltaTime, state.player, state.enemies, 1, state.map);
    if (state.player->invuln > 0) state.player->invuln--;

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++)
    {
        state.enemies[i].Update(deltaTime, state.player, state.player, 1, state.map);
        if (state.enemies[i].invuln > 0) state.enemies[i].invuln--;
    }

    if (state.player->entityHit() && state.player->invuln <= 0)
    {
        state.lives = state.lives - 1;
        if (state.lives <= 0)
        {
            state.gameLost = true;
            state.lives = 0;
            state.player->isActive = false;
        }
        else state.player->invuln = 5000;
    }

    if (state.gameLost && state.player->isActive == false)
    {
        state.gameWon = true;
    }

    if (state.nextScene >= 0) {
        
    }

    if (state.player->position.x >= 18)
    {
        state.nextScene = 1;
    }

}
void Level1::Render(ShaderProgram* program) {
	state.map->Render(program);
	state.player->Render(program);
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++)
    {
        state.enemies[i].Render(program);
    }
}

void Level1::setLives(int lives)
{
    state.lives = lives;
}

int Level1::getLives()
{
    return state.lives;
}

bool Level1::isGameWon()
{
    return state.gameWon;
}

bool Level1::isGameLost()
{
    return state.gameLost;
}