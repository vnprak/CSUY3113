#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
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
Mix_Chunk* bounce;

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool gameWon = false;
bool gameLost = false;
bool gameStart = false;
int lives = 3;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene* currentScene;
Scene* sceneList[1];


GLuint fontTextureID;

void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Can't spell bad encapsulation without psula", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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

    bounce = Mix_LoadWAV("MW4 - WEAPON Railgun.wav");
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

    if (keys[SDL_SCANCODE_LEFT]) {
        currentScene->state.player->movement.x = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.player->movement.x = 1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animRight;
    }
    else if (keys[SDL_SCANCODE_UP]) {
        currentScene->state.player->movement.y = 1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animUp;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        currentScene->state.player->movement.y = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animDown;
    }

    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

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

    if (currentScene->state.player->position.x > 15)
    {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-15, 0, 0));
    }
    else if (currentScene->state.player->position.x > 5)
    {
        viewMatrix = glm::translate(viewMatrix,
            glm::vec3(-currentScene->state.player->position.x, 0, 0));
    }
    else
    {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 0, 0));
    }


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
        Util::DrawText(&program, fontTextureID, "Press enter to start", 0.5f, -0.25f, glm::vec3(-2.25, -1.0, 0.0));
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
        std::string lifeString = life + std::to_string(lives);
        if (currentScene->state.player->position.x > 15) {
            Util::DrawText(&program, fontTextureID, lifeString, 0.5f, -0.25f, glm::vec3(11, -0.5f, 0.0f));
            Util::DrawText(&program, fontTextureID, pos, 0.5f, -0.25f, glm::vec3(currentScene->state.player->position.x - 4.0f, currentScene->state.player->position.y, 0.0f));
        }
        else if (currentScene->state.player->position.x > 5) {
            Util::DrawText(&program, fontTextureID, lifeString, 0.5f, -0.25f, glm::vec3(currentScene->state.player->position.x - 4.0f, -0.5f, 0.0f));
            Util::DrawText(&program, fontTextureID, pos, 0.5f, -0.25f, glm::vec3(currentScene->state.player->position.x - 4.0f, currentScene->state.player->position.y, 0.0f));
        }
        else {
            Util::DrawText(&program, fontTextureID, lifeString, 0.5f, -0.25f, glm::vec3(1.0f, -0.5f, 0.0f));
            Util::DrawText(&program, fontTextureID, pos, 0.5f, -0.25f, glm::vec3(1.0f, currentScene->state.player->position.y, 0.0f));

        }
        if (gameLost) Util::DrawText(&program, fontTextureID, "GAME OVER", 0.75f, -0.25f, glm::vec3(currentScene->state.player->position.x - 2.0f, -3.5f, 0.0f));
        else if (gameWon) Util::DrawText(&program, fontTextureID, "YOU WIN!!", 0.75f, -0.25f, glm::vec3(13.0f, -3.5f, 0.0f));
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