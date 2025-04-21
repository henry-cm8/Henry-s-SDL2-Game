#ifndef ENGINE_H
#define ENGINE_H
#include "SDL.h"


#include "SDL_image.h"
#include "Player.h"

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

        //Uint32 lastTick;
        //float deltaTime;


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
