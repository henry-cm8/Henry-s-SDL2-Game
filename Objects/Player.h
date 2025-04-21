#ifndef PLAYER_H
#define PLAYER_H
#include "SDL.h"
#include "SDL_image.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

class Player
{
    public:
        Player(SDL_Renderer* renderer);

        void HandleInput();
        void Update(Uint32 currentTime);
        void Render(SDL_Renderer* renderer);

        int frame;
        int frameDelay;
        Uint32 lastFrameTime;
        int frameWidth;
        int frameHeight;
        int numFrames;

    private:
        SDL_Rect srcRect;
        SDL_Rect dstRect;
        SDL_Rect collisionBox;
        SDL_Texture* playerTex;
        SDL_Texture* idleTex;


        float speed;
        int velX, velY;

};

#endif // PLAYER_H
