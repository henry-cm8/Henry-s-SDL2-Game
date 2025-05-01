#ifndef ENGINE_H
#define ENGINE_H
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
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
        void Reset();
        int score = 0; //=number of opponents offscreen

        bool CheckCollision(const SDL_Rect& l, const SDL_Rect& r);

        Uint32 lastFrame;
        Uint32 currentTime;
        float deltaTime;

        std::vector <Enemy*> enemies;
        std::vector <GameObject*> gameObjects;

        Uint32 lastSpawnTime = 0;
        Uint32 spawnInterval = 1000;

        bool gameOver = false;
        //Score
        TTF_Font* scoreFont;
        SDL_Texture* scoreTexture;
        SDL_Rect scoreRect = {100,0,200,70};

        //Game State
        enum class GameState { MENU, PLAYING, PAUSED, GAMEOVER };

        GameState currentGameState = GameState::MENU;

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
