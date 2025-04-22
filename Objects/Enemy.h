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
        bool IsOffScreen() const;
        SDL_Rect GetRect() const;
        SDL_Rect GetCollisionBox() const override;

        bool tackled = false;
        bool shocked = false;

        SDL_Texture* shockedTex;

    private:
        SDL_Texture* enemyTex;
        SDL_Rect srcRect, dstRect, collisionBox;

        float posX, posY, speed;

        int frame, frameDelay;
        Uint32 lastFrameTime;
        int numFrames, frameWidth, frameHeight;

};

#endif // ENEMY_H
