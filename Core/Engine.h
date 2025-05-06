#ifndef ENGINE_H
#define ENGINE_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

#include "Player.h"
#include "Enemy.h"
#include "GameObject.h"
#include "Button.h"

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
        TTF_Font* scoreFont = nullptr;
        SDL_Texture* scoreTexture = nullptr;
        SDL_Rect scoreRect;

        bool muted = false;

        //Game State
        enum class GameState { MENU, PLAYING, PAUSED, GAMEOVER, INSTRUCTION };

        GameState currentGameState = GameState::MENU;

        //Buttons
        Button* playButton = nullptr;
        Button* instructionButton = nullptr;
        Button* backButton = nullptr;
        Button* quitButton = nullptr;
        Button* againButton = nullptr;

        SDL_Texture* gameOverLineTexture;
        SDL_Texture* scoreLineTexture;
        SDL_Rect gameOverLineRect, scoreLineRect;

        //Sounds
        Mix_Music* cantDelBarca = nullptr;
        Mix_Music* crowdCheering = nullptr;
        Mix_Music* halaMadrid = nullptr;
        bool cantDelBarcaPlaying = false, crowdCheeringPlaying = false, halaMadridPlaying = false, quemirasboboPlaying = false;
        Mix_Chunk* movementSFX = nullptr;
        Mix_Chunk* quemirasbobo = nullptr;

    private:
        bool running;
        SDL_Window* window;
        SDL_Renderer* renderer;

        //Textures/Backgrounds
        SDL_Texture* background;
        SDL_Texture* instruction;

        //Load FootballField
        SDL_Texture* fieldtextureA;
        SDL_Texture* fieldtextureB;
        SDL_Rect fieldrectA;
        SDL_Rect fieldrectB;
        float screenSpeed;

        Player* messi;

};

#endif // ENGINE_H
