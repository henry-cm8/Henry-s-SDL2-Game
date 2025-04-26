#ifndef ENEMY_H
#define ENEMY_H
#include "SDL.h"
#include "SDL_image.h"
#include "GameObject.h"

class Enemy : public GameObject
{
    public:
        Enemy(SDL_Renderer* renderer);
        ~Enemy();

        void Update(Uint32 currentTime, float deltaTime) override;
        void Render(SDL_Renderer* renderer) override;

        SDL_Rect GetRect() const;
        SDL_Rect GetCollisionBox() const override;

        //States
        bool tackled = false;
        bool shocked = false;
        bool IsOffScreen() const;
        bool bounced = false;
        bool passedPlayer = false;

        SDL_Texture* shockedTex;

    private:
        SDL_Texture* enemyTex;
        SDL_Rect srcRect, dstRect, collisionBox;

        float posX, posY, speed;
        float direction;
        int frame, frameDelay;
        Uint32 lastFrameTime;
        int numFrames, frameWidth, frameHeight;

};

#endif // ENEMY_H
