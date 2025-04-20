#include "Player.h"

Player::Player(SDL_Renderer* renderer)
{
    playerTex = IMG_LoadTexture(renderer, "assets/player/messi.png");

    srcRect = {0, 0, 180, 180};
    dstRect = {1100, 270, 180, 180};
    speed = 400;

}

void Player::HandleInput(const Uint8* keystates)
{
    bool moving = false;
    if (keystates[SDL_SCANCODE_UP]) { velY = -1 ; moving = true; }
    if (keystates[SDL_SCANCODE_DOWN]) { velY = 1 ; moving = true; }
    if (keystates[SDL_SCANCODE_RIGHT]) { velX = 1; moving = true; }
    if (keystates[SDL_SCANCODE_LEFT]) { velX = -1; moving = true; }

}

void Player::Update(float deltaTime)
{

    //Position
    dstRect.x += (velX*speed*deltaTime);
    dstRect.y += (velY*speed*deltaTime);

}

void Player::Render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, playerTex, &srcRect, &dstRect);
}
