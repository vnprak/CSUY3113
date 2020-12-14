#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#define TIMER 120
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"


#include "Util.h"
#include "Map.h"
#include "Entity.h"
#include "Scene.h"
#include "Level1.h"


Mix_Music* music;
Mix_Chunk* fire;

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool gameWon = false;
bool gameLost = false;
bool gameStart = false;
int lives = 1;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene* currentScene;
Scene* sceneList[1];


GLuint fontTextureID;

void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}

float levelTime = TIMER * 1000; //120000ms = 120sec
float endTime = SDL_GetTicks();
float currTime = levelTime - endTime;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Objective: SURVIVE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif
    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");


    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    sceneList[0] = new Level1();
    SwitchToScene(sceneList[0]);


    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    music = Mix_LoadMUS("16_Hot Blooded - Gunner's Theme.mp3");
    Mix_PlayMusic(music, -1);

    fire = Mix_LoadWAV("MW4 - WEAPON Railgun.wav");
    Mix_VolumeChunk(fire, MIX_MAX_VOLUME * 0.05f);
    fontTextureID = Util::LoadTexture("font1.png");
}

void ProcessInput() {

    currentScene->state.player->movement = glm::vec3(0);
    currentScene->state.player->acceleration = glm::vec3(0, -9.81f, 0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                // Move the player left
                break;

            case SDLK_RIGHT:
                // Move the player right
                break;

            case SDLK_SPACE:

                break;
            case SDLK_RETURN:
                if (!gameStart)
                {
                    gameStart = true;
                }
                break;
            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (!(gameLost || gameWon)) {
        if (keys[SDL_SCANCODE_SPACE]) {
            if (/*currentScene->getProjectiles() < currentScene->projectileMax()*/ 1 == 1) //I decided that instead of trying to keep track of how many projectiles are on the screen that I would just attempt to fire
                //through the array until none were left and at that point it would just misfire until bullets were available again
            {
                switch (currentScene->state.player->direction)
                {
                case LEFT:
                    currentScene->state.player->animIndices = currentScene->state.player->shootLeft;
                    Mix_PlayChannel(-1, fire, 0);
                    break;
                case RIGHT:
                    currentScene->state.player->animIndices = currentScene->state.player->shootRight;
                    Mix_PlayChannel(-1, fire, 0);
                    break;
                case UP:
                    currentScene->state.player->animIndices = currentScene->state.player->shootUp;
                    Mix_PlayChannel(-1, fire, 0);
                    break;
                case DOWN:
                    currentScene->state.player->animIndices = currentScene->state.player->shootDown;
                    Mix_PlayChannel(-1, fire, 0);
                    break;
                }

                for (int i = 0; i < currentScene->projectileMax(); i++)
                {
                    if (currentScene->state.projectiles[i].isActive == false)
                    {
                        currentScene->state.player->fire(&currentScene->state.projectiles[i]);
                        currentScene->setProjectiles(currentScene->getProjectiles() + 1);
                        break;
                    }
                }
            }
        }

        else if (keys[SDL_SCANCODE_LEFT]) {
            currentScene->state.player->movement.x = -1.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animLeft;
            currentScene->state.player->direction = LEFT;
        }
        else if (keys[SDL_SCANCODE_RIGHT]) {
            currentScene->state.player->movement.x = 1.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animRight;
            currentScene->state.player->direction = RIGHT;
        }
        else if (keys[SDL_SCANCODE_UP]) {
            currentScene->state.player->movement.y = 1.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animUp;
            currentScene->state.player->direction = UP;
        }
        else if (keys[SDL_SCANCODE_DOWN]) {
            currentScene->state.player->movement.y = -1.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animDown;
            currentScene->state.player->direction = DOWN;
        }

        if (glm::length(currentScene->state.player->movement) > 1.0f) {
            currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
        }
    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    endTime = SDL_GetTicks();
    currTime = levelTime - endTime;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    if (currentScene->state.player->entityHit())
    {
        gameLost = true;
    }

    else if (currTime <= 0)
    {
        gameWon = true;
    }

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;

    viewMatrix = glm::mat4(1.0f);

    /*if (currentScene->state.player->position.x > 15)
    {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-15, 0, 0));
    }
    else if (currentScene->state.player->position.x > 5)*/
    {
        viewMatrix = glm::translate(viewMatrix,
            glm::vec3(-currentScene->state.player->position.x, 0, 0));
    }
    /*else
    {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 0, 0));
    }*/


    /*if (currentScene->state.player->position.y > -3.75)*/ viewMatrix = glm::translate(viewMatrix,
        glm::vec3(0, -currentScene->state.player->position.y, 0));

    //else viewMatrix = glm::translate(viewMatrix,
    //    glm::vec3(0, 3.75, 0));

}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (!gameStart) {
        viewMatrix = glm::mat4(1.0f);
        Util::DrawText(&program, fontTextureID, "Super Good Game", .75f, -0.3f, glm::vec3(-3.0, 0.05, 0.0));
        Util::DrawText(&program, fontTextureID, "Press enter to start.", 0.5f, -0.25f, glm::vec3(-2.25, -1.0, 0.0));
        Util::DrawText(&program, fontTextureID, "Objective: SURVIVE. (2 min.)", 0.5f, -0.25f, glm::vec3(-2.0, -2.0, 0.0));
    }
    program.SetViewMatrix(viewMatrix);

    if (gameStart) {
        currentScene->Render(&program);
        glm::vec3 position = currentScene->state.player->position;
        std::string xpos = std::to_string((float)position.x);
        std::string ypos = std::to_string((float)position.y);
        std::string zpos = std::to_string((float)position.z);
        std::string pos = "X = " + xpos + ", Y = " + ypos + ", Z = " + zpos;
        std::string life = "Lives: ";
        std::string time = "Time (sec): ";
        std::string lifeString = life + std::to_string(lives);
        std::string currTimeStr = time + std::to_string((int)currTime/1000);
        /*if (currentScene->state.player->position.x > 15) {
            Util::DrawText(&program, fontTextureID, lifeString, 0.5f, -0.25f, glm::vec3(11, -0.5f, 0.0f));
            Util::DrawText(&program, fontTextureID, pos, 0.5f, -0.25f, glm::vec3(currentScene->state.player->position.x - 4.0f, currentScene->state.player->position.y, 0.0f));
        }
        else if (currentScene->state.player->position.x > 5) */{
            Util::DrawText(&program, fontTextureID, currTimeStr, 0.5f, -0.25f, glm::vec3(currentScene->state.player->position.x - 4.0f, currentScene->state.player->position.y +3.5f, 0.0f));
            //Util::DrawText(&program, fontTextureID, pos, 0.5f, -0.25f, glm::vec3(currentScene->state.player->position.x - 4.0f, currentScene->state.player->position.y, 0.0f));
        }
        /*else {
            Util::DrawText(&program, fontTextureID, lifeString, 0.5f, -0.25f, glm::vec3(1.0f, -0.5f, 0.0f));
            Util::DrawText(&program, fontTextureID, pos, 0.5f, -0.25f, glm::vec3(1.0f, currentScene->state.player->position.y, 0.0f));

        }*/
        if (gameLost) Util::DrawText(&program, fontTextureID, "GAME OVER", 0.75f, -0.25f, glm::vec3(currentScene->state.player->position.x - 2.0f, currentScene->state.player->position.y -0.5f, 0.0f));
        else if (gameWon) Util::DrawText(&program, fontTextureID, "YOU WIN!!", 0.75f, -0.25f, glm::vec3(currentScene->state.player->position.x - 2.0f, currentScene->state.player->position.y -0.5f, 0.0f));
    }

    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        if (gameStart && !gameLost && !gameWon) Update();
        if (currentScene->isGameWon())
        {
            gameWon = true;
        }
        if (currentScene->isGameLost())
        {
            gameLost = true;
        }
        Render();
    }

    Shutdown();
    return 0;
}