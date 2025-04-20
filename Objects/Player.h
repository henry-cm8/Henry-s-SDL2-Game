#ifndef PLAYER_H
#define PLAYER_H
#include "SDL.h"
#include "SDL_image.h"


class Player
{
    public:
        Player(SDL_Renderer* renderer);

        void HandleInput(const Uint8* keystates);
        void Update(float deltaTime);
        void Render(SDL_Renderer* renderer);



    private:
        SDL_Rect srcRect;
        SDL_Rect dstRect;
        SDL_Texture* playerTex;


        float speed;
        int velX, velY;

};

#endif // PLAYER_H
