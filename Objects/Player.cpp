#include "Player.h"

Player::Player(SDL_Renderer* renderer)
{
    playerTex = IMG_LoadTexture(renderer, "assets/player/messitest.png");

    srcRect = {0, 0, 180, 180};
    dstRect = {1100, 270, 180, 180};
    speed = 10;

}

void Player::HandleInput(const Uint8* keystates)
{

    if (keystates[SDL_SCANCODE_UP]) {
        dstRect.y -= speed;
    }
    if (keystates[SDL_SCANCODE_DOWN]) {
        dstRect.y += speed;
    }
    if (keystates[SDL_SCANCODE_RIGHT]) {
        dstRect.x += speed;
    }
    if (keystates[SDL_SCANCODE_LEFT]) {
        dstRect.x -= speed;
    }

}

void Player::Update()
{

    //Position
    //dstRect.x += (velX*speed);
    //dstRect.y += (velY*speed);

}

void Player::Render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, playerTex, &srcRect, &dstRect);
}
