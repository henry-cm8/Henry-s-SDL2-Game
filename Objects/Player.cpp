#include "Player.h"

Player::Player(SDL_Renderer* renderer)
{
    playerTex = IMG_LoadTexture(renderer, "assets/player/messitest.png");

    srcRect = {0, 0, 180, 180};
    dstRect = {1100, 270, 180, 180};
    collisionBox = {dstRect.x, dstRect.y+90, 90, 90};

    speed = 1;

}

void Player::HandleInput(SDL_Event e)
{

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
        switch (e.key.keysym.sym)
        {
            case SDLK_UP:
            case SDLK_DOWN:
            case SDLK_LEFT:
            case SDLK_RIGHT:
                velX = 0; // stop horizontal movement
                velY = 0;
                break;
        }
    }


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
