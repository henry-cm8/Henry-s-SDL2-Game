#include "Player.h"

Player::Player(SDL_Renderer* renderer)
{
    playerTex = IMG_LoadTexture(renderer, "assets/player/messi.png");
    idleTex = IMG_LoadTexture(renderer, "assets/player/messistill.png");
    rightTex = IMG_LoadTexture(renderer, "assets/player/messiright.png");
    upTex = IMG_LoadTexture(renderer, "assets/player/messiup.png");
    downTex = IMG_LoadTexture(renderer, "assets/player/messidown.png");

    frame = 0;
    frameDelay = 200;
    lastFrameTime = 0;
    frameWidth = 180;
    frameHeight = 180;

    numFramesLeft = 4;
    numFramesRight = 2;


    srcRect = {0, 0, 180, 180};
    dstRect = {1100, 270, 180, 180};
    collisionBox = {dstRect.x, dstRect.y+90, 90, 90};

    speed = 1;

}

void Player::HandleInput()
{
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    velX = 0;
    velY = 0;

    if (keystate[SDL_SCANCODE_LEFT]) velX = -speed;
    if (keystate[SDL_SCANCODE_RIGHT]) velX = speed;
    if (keystate[SDL_SCANCODE_UP]) velY = -speed;
    if (keystate[SDL_SCANCODE_DOWN]) velY = speed;

}

void Player::Update(Uint32 currentTime)
{

    //Position
    dstRect.x += velX;
    dstRect.y += velY;

    //Animation
    if (velX < 0)
    {
        //Left
        if (currentTime > lastFrameTime + frameDelay)
        {
            frame = (frame+1)% numFramesLeft;
            lastFrameTime = currentTime;
        }
    }
    else if (velX > 0)
    {
        //Right
        if (currentTime > lastFrameTime + frameDelay)
        {
            frame = (frame+1)% numFramesRight;
            lastFrameTime = currentTime;
        }
    }

    else
    {
        frame = 0;
    }

    //Wall detection
    if (dstRect.y < 90) dstRect.y = 90;
    if (dstRect.y + dstRect.h > SCREEN_HEIGHT) dstRect.y = SCREEN_HEIGHT - dstRect.h;
    if (dstRect.x < 0) dstRect.x = 0;
    if (dstRect.x > SCREEN_WIDTH-dstRect.w) dstRect.x = SCREEN_WIDTH - dstRect.w;

}

void Player::Render(SDL_Renderer* renderer)
{
    if (velX == 0 && velY == 0)
    {
        //idle
        srcRect.x = 0;
        SDL_RenderCopy(renderer, idleTex, &srcRect, &dstRect);
    }
    else if (velX < 0)
    {
        //Left
        srcRect.x = frame * frameWidth;
        SDL_RenderCopy(renderer, playerTex, &srcRect, &dstRect);
    }
    else if (velX > 0)
    {
        //Right
        srcRect.x = frame * frameWidth;
        SDL_RenderCopy(renderer, rightTex, &srcRect, &dstRect);
    }
    else if (velY < 0)
    {
        //Up
        srcRect.x = 0;
        SDL_RenderCopy(renderer, upTex, &srcRect, &dstRect);
    }
    else if (velY > 0)
    {
        //Down
        srcRect.x = 0;
        SDL_RenderCopy(renderer, downTex, &srcRect, &dstRect);
    }

}
