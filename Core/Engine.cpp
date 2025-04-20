#include "Engine.h"

bool Engine::Init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("Failed to Initialize SDL: %s", SDL_GetError());
        return false;
    }
    //Create Window
    window = SDL_CreateWindow("Henry's Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }
    //Create Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    running = true;
    return true;
}

void Engine::HandleEvents()
{

}

void Engine::Update()
{

}

void Engine::Render()
{

}

void Engine::Clean()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
