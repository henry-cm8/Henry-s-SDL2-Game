#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "SDL.h"
#include "SDL_image.h"

class GameObject
{
    public:
        virtual ~GameObject() {}
        virtual void Update(Uint32 currentTime, float deltaTime) = 0;
        virtual void Render(SDL_Renderer* renderer) = 0;
        virtual SDL_Rect GetCollisionBox() const = 0;


};

#endif // GAMEOBJECT_H
