#ifndef ENEMY_H
#define ENEMY_H
#include "SDL.h"
#include "SDL_image.h"


class Enemy
{
    public:
        Enemy(SDL_Renderer* renderer);
        ~Enemy();

        void Update(Uint32 currentTime, float deltaTime);
        void Render(SDL_Renderer* renderer);
        bool IsOffScreen() const;
        SDL_Rect GetRect() const;

    private:
        SDL_Texture* enemyTex;
        SDL_Rect srcRect, dstRect, collisionBox;

        float posX, posY, speed;

        int frame, frameDelay;
        Uint32 lastFrameTime;
        int numFrames, frameWidth, frameHeight;

};

#endif // ENEMY_H
