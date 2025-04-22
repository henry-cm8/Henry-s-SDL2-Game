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

    speed = 450.0f;

    srcRect = {0, 0, frameWidth, frameHeight};

    //Field bound
    //int minY = 0;
    //int maxY = 720 - frameHeight;
    //posY = minY + rand() % (maxY - minY + 1);
    posY = 45 + (rand()%3)*225;
    posX = -225;

    shockedTex = IMG_LoadTexture(renderer, "assets/enemy/ramosshocked.png");

    dstRect = { static_cast<int>(posX), static_cast<int>(posY), 225, 225};
    collisionBox = {dstRect.x+135, dstRect.y+180, 45, 45};
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


        posX += (speed*deltaTime);
        dstRect.x = static_cast<int>(posX);


        //Animation Tackle
        if (currentTime > lastFrameTime + frameDelay)
        {
            frame = (frame+1) % numFrames;
            lastFrameTime = currentTime;
        }
        srcRect.x = frame * frameWidth;

        //Collision
        collisionBox.x = dstRect.x+135;
        collisionBox.y = dstRect.y+180;
    }

    else
    {
        enemyTex = shockedTex;
        srcRect.x = 0;
    }
}

void Enemy::Render(SDL_Renderer* renderer) //override
{
    SDL_RenderCopy(renderer, enemyTex, &srcRect, &dstRect);
}

bool Enemy::IsOffScreen() const
{
    return dstRect.x > 1280;
}

SDL_Rect Enemy::GetRect() const //override
{
    return dstRect;
}

SDL_Rect Enemy::GetCollisionBox() const
{
    return collisionBox;
}
