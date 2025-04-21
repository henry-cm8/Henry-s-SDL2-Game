#include "Player.h"

Player::Player(SDL_Renderer* renderer)
{
    playerTex = IMG_LoadTexture(renderer, "assets/player/messi.png");
    idleTex = IMG_LoadTexture(renderer, "assets/player/messistill.png");

    frame = 0;
    frameDelay = 200;
    lastFrameTime = 0;
    frameWidth = 180;
    frameHeight = 180;
    numFrames = 4;

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
    if (velX != 0 || velY != 0)
    {
        //Player moving
        if (currentTime > lastFrameTime + frameDelay)
        {
            frame = (frame+1)% numFrames;
            lastFrameTime = currentTime;
        }
    } else
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
    else
    {
        srcRect.x = frame * frameWidth;
        SDL_RenderCopy(renderer, playerTex, &srcRect, &dstRect);
    }
}
