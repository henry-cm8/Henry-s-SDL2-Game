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
    collisionBox = {dstRect.x, dstRect.y+135, 45, 45};

    speed = 500.0f;

}

void Player::HandleInput()
{
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    velX = 0;
    velY = 0;

    if (keystate[SDL_SCANCODE_LEFT]) velX = -1;
    if (keystate[SDL_SCANCODE_RIGHT]) velX = 1;
    if (keystate[SDL_SCANCODE_UP]) velY = -1;
    if (keystate[SDL_SCANCODE_DOWN]) velY = 1;

}

void Player::Update(Uint32 currentTime, float deltaTime)
{

    //Position
    posX = dstRect.x;
    posY = dstRect.y;

    posX += (velX*speed*deltaTime);
    posY += (velY*speed*deltaTime);

    dstRect.x = static_cast<int>(posX);
    dstRect.y = static_cast<int>(posY);

    //Collision box
    collisionBox.x = dstRect.x;
    collisionBox.y = dstRect.y+135;

    if ((velX != 0 || velY != 0) && currentTime > lastFrameTime + frameDelay)
    {
        if (velX < 0) frame = (frame+1) % numFramesLeft; //leftward
        else if (velX > 0) frame = (frame+1) % numFramesRight; //rightward
        else frame=0; //vertical and idle uses single frame
        lastFrameTime = currentTime;
    }

    //Wall detection
    if (collisionBox.y < 180) dstRect.y = 45; //top
    if (collisionBox.y + collisionBox.h > SCREEN_HEIGHT) dstRect.y = SCREEN_HEIGHT - dstRect.h; //bottom
    if (collisionBox.x < 0) dstRect.x = 0;
    if (collisionBox.x > SCREEN_WIDTH-dstRect.w) dstRect.x = SCREEN_WIDTH - dstRect.w;

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
