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

    lastFrame = SDL_GetTicks();

    //Load FootballField
    fieldtexture = IMG_LoadTexture(renderer, "assets/footballfield.jpg");
    fieldrect = {0, 180, 1280, 540};

    //Player
    messi = new Player(renderer);
    messi->Render(renderer);

    //DeltaTime


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

    }

     messi->HandleInput();
}

void Engine::Update()
{
    Uint32 currentTime = SDL_GetTicks();
    deltaTime = (currentTime - lastFrame) / 1000.0f; //to seconds

    std::cout<<deltaTime<<std::endl;

    //Spawn enemies
    if (currentTime > lastSpawnTime + spawnInterval)
    {
        //spawn new enemy
        enemies.push_back(new Enemy(renderer));
        lastSpawnTime = currentTime;
    }
    //Update enemies
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        (*it)->Update(currentTime, deltaTime);

        //Check collision
        if (CheckCollision(messi->GetCollisionBox(), (*it)->GetCollisionBox()))
        {
            running = false;
        }

        if ((*it)->IsOffScreen())
        {
            delete *it;
            it = enemies.erase(it); //remove enemy from list
            //add score later
        }
        else
        {
            ++it;
        }
    }

    lastFrame = currentTime;

    messi->Update(currentTime, deltaTime);



}

void Engine::Render()
{
    //Football Field: Background
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, fieldtexture, nullptr, &fieldrect);

    //messi->Render(renderer);
    for (Enemy* e : enemies)
    {
        if (e->GetCollisionBox().y + e->GetCollisionBox().h <= messi->GetCollisionBox().y)
        {
            e->Render(renderer);
            messi->Render(renderer);
        }
        else if (e->GetCollisionBox().y + e->GetCollisionBox().h > messi->GetCollisionBox().y)
        {
            messi->Render(renderer);
            e->Render(renderer);
        }

    }
        //e->Render(renderer);

    SDL_RenderPresent(renderer);
}

bool Engine::CheckCollision(const SDL_Rect& a, const SDL_Rect& b)
{
    return SDL_HasIntersection(&a, &b);
}

void Engine::Clean()
{
    delete messi;
    for (Enemy* e : enemies)
    {
        delete e;
    }
    enemies.clear();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
