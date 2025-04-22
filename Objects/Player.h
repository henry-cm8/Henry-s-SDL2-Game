#ifndef PLAYER_H
#define PLAYER_H
#include "SDL.h"
#include "SDL_image.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#include "GameObject.h"

class Player : public GameObject
{
    public:
        Player(SDL_Renderer* renderer);

        void HandleInput(SDL_Event& e);
        void Update(Uint32 currentTime, float deltaTime) override;
        void Render(SDL_Renderer* renderer) override;

        int frame;
        int frameDelay;
        Uint32 lastFrameTime;
        int frameWidth;
        int frameHeight;

        int numFramesLeft;
        int numFramesRight;
        //int numFramesUp;
        //int numFramesDown;
        float posX;
        float posY;

        int currentRow;
        std::vector<int> rowY;
        float targetY;
        bool isMoving = false;

        SDL_Rect GetCollisionBox() const override;
        SDL_Rect GetRect() const;

        bool isDead = false;
        SDL_Texture* deadTex;
        int deadFrame = 0;
        int numFramesDead = 8;

    private:
        SDL_Rect srcRect;
        SDL_Rect dstRect;
        SDL_Rect collisionBox;
        SDL_Texture* playerTex;
        SDL_Texture* idleTex;
        SDL_Texture* rightTex;
        SDL_Texture* upTex;
        SDL_Texture* downTex;


        float speed;
};

#endif // PLAYER_H
