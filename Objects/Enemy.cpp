#include "Enemy.h"
#include <cstdlib>
#include <ctime>

Enemy::Enemy(SDL_Renderer* renderer)
{
    enemyTex = IMG_LoadTexture(renderer, "assets/enemy/ramostackle.png");

    frameWidth = 180;
    frameHeight = 180;
    numFrames = 8;
    frame = 0;
    frameDelay = 100;
    lastFrameTime = 0;

    speed = 500.0f;

    srcRect = {0, 0, frameWidth, frameHeight};

    posY = 45 + (rand()%3)*225;
    posX = -225;

    shockedTex = IMG_LoadTexture(renderer, "assets/enemy/ramosshocked.png");

    direction = 1.0f;

    dstRect = { static_cast<int>(posX), static_cast<int>(posY), 225, 225};
    collisionBox = {dstRect.x+180, dstRect.y+180, 45, 45};
}

Enemy::~Enemy()
{
    SDL_DestroyTexture(enemyTex);
}

void Enemy::Update(Uint32 currentTime, float deltaTime) //override
{
    //Move right
    if (!shocked && !tackled)
    {
        //Update direction
        if (dstRect.x + dstRect.w >= SCREEN_WIDTH)
        {
            dstRect.x = SCREEN_WIDTH - dstRect.w;
            direction = -1.0f;
            bounced = true;
        }
        //Update position
        posX += (speed*deltaTime*direction);
        dstRect.x =  static_cast<int>(posX);

        //Animation when running
        if (currentTime > lastFrameTime + frameDelay)
        {
            frame = (frame+1) % numFrames;
            lastFrameTime = currentTime;
        }
        srcRect.x = frame * frameWidth;

        //Collision
        collisionBox.x = (direction == -1.0f)? dstRect.x : dstRect.x + 180;
        collisionBox.y = dstRect.y+180;
    }
    else if (shocked && tackled)
    {
        srcRect.x = (numFrames-2)*frameWidth;
    }
    else if (shocked && !tackled)
    {
        enemyTex = shockedTex;
        srcRect.x = 0;
    }
}

void Enemy::Render(SDL_Renderer* renderer) //override
{
    SDL_RenderCopyEx(renderer, enemyTex, &srcRect, &dstRect, 0, nullptr, (direction == -1.0f)? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

bool Enemy::IsOffScreen() const
{
    return (dstRect.x <= -dstRect.w && direction == -1.0f);
}

SDL_Rect Enemy::GetRect() const //override
{
    return dstRect;
}

SDL_Rect Enemy::GetCollisionBox() const
{
    return collisionBox;
}
