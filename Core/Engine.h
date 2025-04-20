#ifndef ENGINE_H
#define ENGINE_H
#include "SDL.h"
#include "SDL_image.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

class Engine
{
    public:

        bool Init();
        bool IsRunning() { return running; }
        void HandleEvents();
        void Update();
        void Render();
        void Clean();



        bool running;
        SDL_Window* window;
        SDL_Renderer* renderer;

        Engine();

};

#endif // ENGINE_H
