#include "Level1.h"

#define LEVEL1_WIDTH 15
#define LEVEL1_HEIGHT 15

#define LEVEL1_ENEMY_COUNT 4
#define PROJECTILE_LIMIT 10

unsigned int level1_data[] =
{
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32,
 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32,
 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32,
 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32,
 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32,
 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32,
 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32,
 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32,
 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32,
 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32,
 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32,
 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32,
 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32
};

void Level1::Initialize() {

    state.nextScene = -1;

	GLuint mapTextureID = Util::LoadTexture("tiles.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 16, 16);
	// Move over all of the player and enemy code from initialization.
    // Initialize Player
    state.player = new Entity();
    state.player->direction = RIGHT;
    state.player->entityType = PLAYER;
    state.player->spawn = glm::vec3(6, -6, 0);
    state.player->position = state.player->spawn;
    state.player->movement = glm::vec3(0);
    state.player->speed = 3.5f;
    state.player->textureID = Util::LoadTexture("MechSheet.png");

    state.player->animRight = new int[4]{ 18, 19, 20, 21 };
    state.player->animLeft = new int[4]{ 42, 43, 44, 45 };
    state.player->animUp = new int[4]{ 30, 31, 32, 33 };
    state.player->animDown = new int[4]{ 7, 8, 9, 6 };

    state.player->shootRight = new int[4]{ 14, 15, 13, 13 };
    state.player->shootLeft = new int[4]{ 38, 39, 37, 37 };
    state.player->shootUp = new int[4]{ 26, 27, 25, 25 };
    state.player->shootDown = new int[4]{ 2, 3, 1, 1 };

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
    state.maxBullets = PROJECTILE_LIMIT;
    GLuint projectileTextureID = Util::LoadTexture("ball.png");

    for (int i = 0; i < PROJECTILE_LIMIT; i++)
    {
        state.projectiles[i].direction = RIGHT;
        state.projectiles[i].entityType = PROJECTILE;
        state.projectiles[i].textureID = projectileTextureID;
        state.projectiles[i].position = glm::vec3(0);
        state.projectiles[i].speed = 3;
        state.projectiles[i].owner = NULL;
        state.projectiles[i].isActive = false;

        state.projectiles[i].animRight = new int[4]{0, 3, 6, 3};

        state.projectiles[i].animIndices = state.projectiles[0].animRight;
        state.projectiles[i].animFrames = 4;
        state.projectiles[i].animIndex = 0;
        state.projectiles[i].animTime = 0;
        state.projectiles[i].animCols = 3;
        state.projectiles[i].animRows = 4;
    }

    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("MechSheet Enemy.png");

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies->position = glm::vec3(12, -3, 0);
        state.enemies[i].speed = 0.0;
        state.enemies[i].aiType = WAITANDGO;
        state.enemies[i].aiState = WALKING;

        state.enemies[i].animRight = new int[4]{ 18, 19, 20, 21 };
        state.enemies[i].animLeft = new int[4]{ 42, 43, 44, 45 };
        state.enemies[i].animUp = new int[4]{ 30, 31, 32, 33 };
        state.enemies[i].animDown = new int[4]{ 7, 8, 9, 6 };

        state.enemies[i].animIndices = state.enemies[i].animRight;
        state.enemies[i].animFrames = 4;
        state.enemies[i].animIndex = 0;
        state.enemies[i].animTime = 0;
        state.enemies[i].animCols = 12;
        state.enemies[i].animRows = 8;

        state.enemies[i].height = 2.0f;
        state.enemies[i].width = 2.0f;
        state.enemies[i].colheight = 1.2f;
        state.enemies[i].colwidth = 1.4f;
    }
    
    state.enemies[0].spawn = glm::vec3(2, -2, 0);
    state.enemies[1].spawn = glm::vec3(12, -2, 0);
    state.enemies[2].spawn = glm::vec3(2, -12, 0);
    state.enemies[3].spawn = glm::vec3(12, -12, 0);

    state.enemies[0].position = glm::vec3(2, -2, 0);
    state.enemies[1].position = glm::vec3(12, -2, 0);
    state.enemies[2].position = glm::vec3(2, -12, 0);
    state.enemies[3].position = glm::vec3(12, -12, 0);
}
void Level1::Update(float deltaTime) {
    state.player->hit = false;
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) state.enemies[i].hit = false;
	state.player->Update(deltaTime, state.player, state.enemies, 1, state.map);
    if (state.player->invuln > 0) state.player->invuln--;

    for (int i = 0; i < PROJECTILE_LIMIT; i++)
    {
        for (int j = 0; j < LEVEL1_ENEMY_COUNT; j++)
        {
            state.projectiles[i].Update(deltaTime, state.player, &state.enemies[j], 1, state.map);
        }
    }

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
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++)
    {
        if (state.enemies[i].hit)
        {
            state.enemies[i].position = state.enemies[i].spawn;
        }
    }
}
void Level1::Render(ShaderProgram* program) {
	state.map->Render(program);
	state.player->Render(program);
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++)
    {
        state.enemies[i].Render(program);
    }
    for (int i = 0; i < PROJECTILE_LIMIT; i++)
    {
        state.projectiles[i].Render(program);
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

int Level1::getProjectiles()
{
    return state.projectileCount;
}

void Level1::setProjectiles(int count)
{
    state.projectileCount = count;
}

int Level1::projectileMax()
{
    return state.maxBullets;
}

int Level1::enemyCount()
{
    return LEVEL1_ENEMY_COUNT;
}