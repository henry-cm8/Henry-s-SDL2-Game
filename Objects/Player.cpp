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

    rowY = {45, 270, 495};
    currentRow = 1;
    posY=rowY[currentRow];

    srcRect = {0, 0, 180, 180};
    dstRect = {800, static_cast<int>(posY), 225, 225};
    collisionBox = {dstRect.x, dstRect.y+205, 125 , 20};
    targetY = posY;

    deadTex = IMG_LoadTexture(renderer, "assets/player/messidead.png");

    speed = 500.0f;

}

void Player::HandleInput(SDL_Event& e)
{
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0 && !isMoving)
    {
        if (e.key.keysym.sym == SDLK_UP && currentRow > 0)
        {
            currentRow--;
            targetY = rowY[currentRow];
            isMoving = true;
        }
        else if (e.key.keysym.sym == SDLK_DOWN && currentRow < 2)
        {
            currentRow++;
            targetY = rowY[currentRow];
            isMoving = true;
        }
    }


}

void Player::Update(Uint32 currentTime, float deltaTime) //override
{
    if (isMoving && !isDead)
    {
        float direction = (targetY > posY)? 1.0f : -1.0f;
        posY += (direction*speed*deltaTime);

        // If reached or passed target, snap and stop moving
        if ((direction > 0 && posY >= targetY) || (direction < 0 && posY <= targetY))
        {
            posY = targetY;
            isMoving = false;
        }

        dstRect.y = static_cast<int>(posY);
        collisionBox.y = dstRect.y+205;
    }
    //Animation
    if ( !isDead && currentTime > lastFrameTime + frameDelay)
    {
        frame = (frame+1) % numFramesLeft; //leftward
        lastFrameTime = currentTime;
    }

    //Death
    if (isDead)
    {
        if (currentTime > lastFrameTime + frameDelay-100)
        {
            deadFrame = (deadFrame+1) % numFramesDead;
            lastFrameTime = currentTime;
        }
        return;
    }

}

void Player::Render(SDL_Renderer* renderer) //override
{
    //Death
    if (isDead)
    {
        srcRect.x = deadFrame * frameWidth;
        SDL_RenderCopy(renderer, deadTex, &srcRect, &dstRect);
        return;
    }

    SDL_Texture* currentTex;
    if (isMoving)
    {
        if (targetY < posY)
            currentTex = upTex; //Up
        else
            currentTex = downTex; //Down
        srcRect.x = 0;
    }
    else
    {
        currentTex = playerTex; //Idle
        srcRect.x = frame * frameWidth;
    }

    SDL_RenderCopy(renderer, currentTex, &srcRect, &dstRect);
}

SDL_Rect Player::GetCollisionBox() const //override
{
    return collisionBox;
}

SDL_Rect Player::GetRect() const
{
    return dstRect;
}

