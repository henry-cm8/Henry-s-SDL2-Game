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
    //SDL_TTF Init
    if (TTF_Init() != 0)
    {
        SDL_Log("Failed to Initialize TTF: %s", SDL_GetError());
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

    //Load Game Background
    background = IMG_LoadTexture(renderer, "assets/gamebackground.png");

    //Load FootballField
    fieldtextureA = IMG_LoadTexture(renderer, "assets/background.png");
    fieldtextureB = IMG_LoadTexture(renderer, "assets/background.png");
    fieldrectA = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    fieldrectB = {fieldrectA.x - SCREEN_WIDTH, fieldrectA.y, fieldrectA.w, fieldrectA.h};
    screenSpeed = 600.0f;

    //Player
    messi = new Player(renderer);
    gameObjects.push_back(messi);

    //Score
    scoreFont = TTF_OpenFont("assets/fonts/HighlandGothicFLF.ttf", 24);
    SDL_Texture* scoreTexture;
    scoreRect = {100,0,200,70};

    //Menu
    playButton = new Button(renderer, scoreFont, "Play", {540, 330, 200, 60}, {255,255,255,255});
    playButton->baseColor = {0,0,102,255};
    playButton->hoverColor = {0,0,255,255};
    playButton->Update();

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

        if (currentGameState == GameState::MENU) {
            playButton->Update();
            playButton->HandleEvent(e);

            if (playButton->IsClicked()) {
                Reset();
                currentGameState = GameState::PLAYING;
            }
        } else if (currentGameState == GameState::PLAYING) {
            messi->HandleInput(e);
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) currentGameState = GameState::MENU;
        }
    }

}

void Engine::Update()
{
    Uint32 currentUpdateTime = SDL_GetTicks();
    deltaTime = (currentUpdateTime - lastFrame) / 1000.0f; //to seconds
    std::cout<<deltaTime<<std::endl;

    if (currentGameState != GameState::PLAYING) return; //Check if PLAYING?

    //Screen floating
    if (!gameOver)
    {
        fieldrectA.x += (screenSpeed*deltaTime);
        fieldrectB = {fieldrectA.x - SCREEN_WIDTH, fieldrectA.y, fieldrectA.w, fieldrectA.h};
        if (fieldrectA.x > SCREEN_WIDTH) fieldrectA.x = 0;
    }

    //Spawn enemies
    if (currentUpdateTime > lastSpawnTime + spawnInterval && !gameOver)
    {
        //spawn new enemy
        Enemy* newEnemy = new Enemy(renderer);
        enemies.push_back(newEnemy);
        gameObjects.push_back(newEnemy);
        std::cout<<"An enemy created"<<std::endl;
        lastSpawnTime = currentUpdateTime;
    }
    //Update enemies
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        Enemy* enemy = *it;

        enemy->Update(currentUpdateTime, deltaTime);

        if (gameOver) enemy->shocked=true;
        //Check collision
        if (CheckCollision(enemy->GetCollisionBox(), messi->GetCollisionBox()))
        {
            gameOver = true;
            messi->isDead=true;
            enemy->tackled=true;
            //Game Over
        }
        //Check score
        if (enemy->bounced && !enemy->passedPlayer && enemy->GetRect().x < messi->GetRect().x)
        {
            score += 1;
            enemy->passedPlayer = true;
        }
        //Deletion
        if (enemy->IsOffScreen())
        {
            auto it_obj = std::find(gameObjects.begin(), gameObjects.end(), enemy);
            if (it_obj != gameObjects.end()) //Remove from gameObjects
                gameObjects.erase(it_obj);
            delete enemy;
            it = enemies.erase(it); //remove from enemy list
            //add score later
        }
        else
        {
            ++it;
        }
    }
    lastFrame = currentUpdateTime;

    //Score Update
    if (scoreTexture != nullptr) {
        SDL_DestroyTexture(scoreTexture);
        scoreTexture = nullptr;
    }
    std::stringstream ss;
    ss << "Opponent cooked: " << score;
    std::string scoreTextureString = ss.str();
    SDL_Color scoreColor = {0,0,0};
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(scoreFont, scoreTextureString.c_str(), scoreColor);
    scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    SDL_SetTextureBlendMode(scoreTexture, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(scoreSurface);

    //Update Player
    messi->Update(currentUpdateTime, deltaTime);
}

void Engine::Render()
{
    //Clear Screen
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);

    if (currentGameState == GameState::MENU)
    {
        SDL_RenderCopy(renderer, background, NULL, NULL);
        playButton->Render(renderer);
    }
    else if (currentGameState == GameState::PLAYING)
    {
        //Football Field: Background
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, fieldtextureA, nullptr, &fieldrectA);
        SDL_RenderCopy(renderer, fieldtextureB, nullptr, &fieldrectB);

        //Score
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

        //Sort gameObjects based on Y(collision box)
        std::sort(gameObjects.begin(), gameObjects.end(), [](GameObject* a, GameObject* b)
                  {
                      return a->GetCollisionBox().y < b->GetCollisionBox().y;
                  });
        //Render all objects
        for (auto obj : gameObjects) obj->Render(renderer);
    }
    SDL_RenderPresent(renderer);
}

bool Engine::CheckCollision(const SDL_Rect& l, const SDL_Rect& r)
{
    return (l.x+l.w >= r.x &&
            l.x+l.w <= r.x+r.w &&
            l.y+l.h == r.y+r.h);
}

void Engine::Clean()
{
    delete messi;
    for (Enemy* e : enemies)
    {
        delete e;
    }
    SDL_DestroyTexture(scoreTexture);
    enemies.clear();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    std::cout<<"You have beaten "<<score<<" defenders."<<std::endl;
}

void Engine::Reset()
{
    //Reset score
    score = 0;
    //Reset player
    delete messi;
    messi = new Player(renderer);
    //Reset all enemies
    for (Enemy* e : enemies) delete e;
    enemies.clear();
    //Clear gameObjects & Add Player
    gameObjects.clear();
    gameObjects.push_back(messi);

    gameOver = false;
    lastSpawnTime = SDL_GetTicks();
    lastFrame = SDL_GetTicks();
}
