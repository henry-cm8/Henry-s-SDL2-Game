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
    //SDL_Mixer Init
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_Log("Failed to Initialize SDL_Mixer: %s", SDL_GetError());
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
    SDL_Color base = {0,0,102,255}; //Dark Blue
    SDL_Color hover = {0,0,255,255}; //Blue
    background = IMG_LoadTexture(renderer, "assets/gamebackground.png");
    playButton = new Button(renderer, scoreFont, "Play Game", {540, 330, 200, 60}, {255,255,255,255});
    playButton->baseColor = base;
    playButton->hoverColor = hover;
    playButton->Update();
    instructionButton = new Button(renderer, scoreFont, "Instructions", {540, 420, 200, 60}, {255,255,255,255});
    instructionButton->baseColor = base;
    instructionButton->hoverColor = hover;
    instructionButton->Update();

    //Instruction
    backButton = new Button(renderer, scoreFont, "Main Menu", { 60, 600, 200, 60}, {255,255,255,255});
    backButton->baseColor = {0,0,102,255};
    backButton->hoverColor = {0,0,255,255};
    instruction = IMG_LoadTexture(renderer, "assets/instruction.png");
    //Quit Button
    quitButton = new Button(renderer, scoreFont, "Quit", { 540, 510, 200, 60}, {255,255,255,255});
    quitButton->baseColor = base;
    quitButton->hoverColor = hover;
    quitButton->Update();
    //Game Over
    againButton = new Button(renderer, scoreFont, "Try Again", {540,500,200,60}, {255,255,255,255});
    againButton->baseColor = base;
    againButton->hoverColor = hover;

    //Sounds
    cantDelBarca = Mix_LoadMUS("assets/audio/cantdelbarca.mp3");
    halaMadrid = Mix_LoadMUS("assets/audio/halamadrid.mp3");
    crowdCheering = Mix_LoadMUS("assets/audio/crowd.mp3");
    movementSFX = Mix_LoadWAV("assets/audio/whoosh.wav");
    quemirasbobo = Mix_LoadWAV("assets/audio/quemirasbobo.wav");

    running = true;
    return true;
}

void Engine::HandleEvents()
{
    SDL_Event e;

    //Position
    if (gameOver)
    {
        //Positions
        quitButton->baseRect.y = 590;
        //Update and Events
        quitButton->Update();
        againButton->Update();
    }

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            running = false;
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_m) {
            muted = !muted;
            if (muted) {
                Mix_VolumeMusic(0); // Mute music
                Mix_Volume(-1, 0);  // Mute sound effects
            } else {
                Mix_VolumeMusic(MIX_MAX_VOLUME); // Restore music volume
                Mix_Volume(-1, MIX_MAX_VOLUME);  // Restore SFX volume
            }
        }

        if (currentGameState == GameState::MENU) {
            Reset();
            if (!cantDelBarcaPlaying) {
                Mix_PlayMusic(cantDelBarca, -1);
                cantDelBarcaPlaying = true;
            }
            playButton->Update();
            playButton->HandleEvent(e);
            instructionButton->Update();
            instructionButton->HandleEvent(e);
            quitButton->baseRect.y = 510;
            quitButton->Update();
            quitButton->HandleEvent(e);
            if (instructionButton->IsClicked()) {
                currentGameState = GameState::INSTRUCTION;
            }
            if (playButton->IsClicked()) {
                Mix_HaltMusic();
                cantDelBarcaPlaying = false;
                currentGameState = GameState::PLAYING;
            }
            if (quitButton->IsClicked()) running = false;
        } else if (currentGameState == GameState::PLAYING) {
            if (!crowdCheeringPlaying) {
                Mix_PlayMusic(crowdCheering, -1);
                crowdCheeringPlaying = true;
            }
            messi->HandleInput(e);
            if (messi->isMoving && !gameOver) {
                if (!Mix_Playing(1)) Mix_PlayChannel(1, quemirasbobo, 0);
            }

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) currentGameState = GameState::MENU;
            if (gameOver) {
                againButton->HandleEvent(e);
                quitButton->HandleEvent(e);

                if (againButton->IsClicked()) {
                        Reset();
                        Mix_HaltMusic();
                        crowdCheeringPlaying = false;
                        currentGameState = GameState::PLAYING;
                }
                if (quitButton->IsClicked()) {
                        Mix_HaltMusic();
                        cantDelBarcaPlaying = false;
                        running = false;
                }
            }
        } else if (currentGameState == GameState::INSTRUCTION) {
            //Update Back Button
            backButton->baseRect.x = 60;
            backButton->baseRect.y = 600;
            backButton->Update();
            backButton->HandleEvent(e);
            if (backButton->IsClicked()) currentGameState = GameState::MENU;
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
            if (!halaMadridPlaying) {
                Mix_PlayMusic(halaMadrid, -1);
                halaMadridPlaying = true;
            }

            //Score update
            std::string gameOverLine = "Game Over!";
            std::string scoreLine = "You have beaten "+ std::to_string(score) + " defenders.";
            SDL_Surface* gameOverLineSurface = TTF_RenderText_Blended(scoreFont, gameOverLine.c_str(), {255,255,255,255});
            SDL_Surface* scoreLineSurface = TTF_RenderText_Blended(scoreFont, scoreLine.c_str(), {255,255,255,255});
            gameOverLineTexture = SDL_CreateTextureFromSurface(renderer, gameOverLineSurface);
            scoreLineTexture = SDL_CreateTextureFromSurface(renderer, scoreLineSurface);
            gameOverLineRect = {(SCREEN_WIDTH-gameOverLineSurface->w)/2, 300, gameOverLineSurface->w, gameOverLineSurface->h};
            scoreLineRect = {(SCREEN_WIDTH-scoreLineSurface->w)/2, gameOverLineRect.y+gameOverLineRect.h+20, scoreLineSurface->w, scoreLineSurface->h};
            SDL_FreeSurface(gameOverLineSurface);
            SDL_FreeSurface(scoreLineSurface);

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
        instructionButton->Render(renderer);
        quitButton->Render(renderer);
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

        if (gameOver)
        {
            SDL_RenderCopy(renderer, gameOverLineTexture, NULL, &gameOverLineRect);
            SDL_RenderCopy(renderer, scoreLineTexture, NULL, &scoreLineRect);
            againButton->Render(renderer);
            quitButton->Render(renderer);
        }
    }
    else if (currentGameState == GameState::INSTRUCTION)
    {
        SDL_RenderCopy(renderer, instruction, NULL,NULL);
        backButton->Render(renderer);
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
    Mix_FreeMusic(cantDelBarca);
    Mix_FreeMusic(halaMadrid);
    Mix_FreeMusic(crowdCheering);
    Mix_FreeChunk(quemirasbobo);
    Mix_FreeChunk(movementSFX);
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

    halaMadridPlaying = false;
    gameOver = false;
    lastSpawnTime = SDL_GetTicks();
    lastFrame = SDL_GetTicks();
}
