#include "Engine.h"


Engine::Engine() {}



bool Engine::Init()
{
    //SDL Init
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Failed to Initialize SDL: %s", SDL_GetError());
        return false;
    }
    //SDL Image Init
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        SDL_Log("Failed to Initialize SDL_image: %s", SDL_GetError());
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
    //Load FootballField
    fieldtexture = IMG_LoadTexture(renderer, "assets/footballfield.jpg");
    fieldrect = {0, 180, 1280, 540};

    //Player
    messi = new Player(renderer);

    //DeltaTime
    lastTick = SDL_GetTicks();
    deltaTime = 0.0f;

    running = true;
    return true;
}

void Engine::HandleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            running = false;
        else
        {
            const Uint8* keystates = SDL_GetKeyboardState(nullptr);
            messi->HandleInput(keystates);
        }
    }
}

void Engine::Update()
{
    Uint32 currentTick = SDL_GetTicks();
    deltaTime = (currentTick - lastTick) / 1000.0f;
    lastTick = currentTick;

    //const Uint8* keystates = SDL_GetKeyboardState(nullptr);
    //messi->HandleInput(keystates);


    messi->Update(deltaTime);
}

void Engine::Render()
{
    //Football Field: Background

    //SDL_RenderCopy(renderer, fieldtexture, nullptr, nullptr);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, fieldtexture, nullptr, &fieldrect);

    messi->Render(renderer);

    SDL_RenderPresent(renderer);
}

void Engine::Clean()
{
    delete messi;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
