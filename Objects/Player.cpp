#include "Player.h"

Player::Player(SDL_Renderer* renderer)
{
    playerTex = IMG_LoadTexture(renderer, "assets/player/messitest.png");

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

    /*
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
            case SDLK_UP:    velY = -speed; break;
            case SDLK_DOWN:  velY = speed;  break;
            case SDLK_LEFT:  velX = -speed; break;
            case SDLK_RIGHT: velX = speed;  break;
        }
    }

    if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        //for smoother movement
        switch (e.key.keysym.sym)
        {
            case SDLK_UP:
            case SDLK_DOWN:
                velY = 0;
                break;

            case SDLK_LEFT:
            case SDLK_RIGHT:
                velX = 0;
                break;
        }
    }
    */
}

void Player::Update()
{

    //Position
    dstRect.x += velX;
    dstRect.y += velY;

    //Wall detection
    if (dstRect.y < 90) dstRect.y = 90;
    if (dstRect.y + dstRect.h > SCREEN_HEIGHT) dstRect.y = SCREEN_HEIGHT - dstRect.h;
    if (dstRect.x < 0) dstRect.x = 0;
    if (dstRect.x > SCREEN_WIDTH-dstRect.w) dstRect.x = SCREEN_WIDTH - dstRect.w;

}

void Player::Render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, playerTex, &srcRect, &dstRect);
}
