#ifndef ENGINE_H
#define ENGINE_H
#include "SDL.h"
#include <iostream>
#include <vector>
#include "SDL_image.h"
#include "Player.h"
#include "Enemy.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

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

        Uint32 lastFrame;
        Uint32 currentTime;
        float deltaTime;

        std::vector <Enemy*> enemies;
        Uint32 lastSpawnTime;
        Uint32 spawnInterval = 1500;


    private:
        bool running;
        SDL_Window* window;
        SDL_Renderer* renderer;

        //Load FootballField
        SDL_Texture* fieldtexture;
        SDL_Rect fieldrect;

        Player* messi;

};

#endif // ENGINE_H
