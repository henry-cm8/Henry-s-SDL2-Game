#ifndef ENGINE_H
#define ENGINE_H
#include "SDL.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

class Engine
{
    public:
        Engine* m_Engine;
        Engine();

        bool Init();
        bool IsRunning() { return running; }
        void HandleEvents();
        void Update();
        void Render();
        void Clean();

        bool running;

        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;

};

#endif // ENGINE_H
