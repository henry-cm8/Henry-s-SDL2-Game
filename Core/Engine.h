#ifndef ENGINE_H
#define ENGINE_H
#include "SDL.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "SDL_image.h"
#include "Player.h"
#include "Enemy.h"
#include "GameObject.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

//enum class GameState

class Engine
{
    public:

        Engine();
        bool Init();
        bool IsRunning() { return running; }
        void HandleEvents();
        void Update();
        void Render();
        void Clean();
        int score = 0; //=number of opponents offscreen

        bool CheckCollision(const SDL_Rect& l, const SDL_Rect& r);

        Uint32 lastFrame;
        Uint32 currentTime;
        float deltaTime;

        std::vector <Enemy*> enemies;
        std::vector <GameObject*> gameObjects;

        Uint32 lastSpawnTime;
        Uint32 spawnInterval = 800;

        bool gameOver = false;


    private:
        bool running;
        SDL_Window* window;
        SDL_Renderer* renderer;

        //Load FootballField
        SDL_Texture* fieldtextureA;
        SDL_Texture* fieldtextureB;
        SDL_Rect fieldrectA;
        SDL_Rect fieldrectB;
        float screenSpeed;

        Player* messi;

};

#endif // ENGINE_H
