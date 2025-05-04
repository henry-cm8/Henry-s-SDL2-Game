#ifndef BUTTON_H
#define BUTTON_H
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>


class Button
{
    public:
        Button(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Rect rect, SDL_Color textcolor);
        ~Button();

        void Render(SDL_Renderer* renderer);
        void HandleEvent(SDL_Event& e);
        bool IsClicked() const { return clicked; }
        void Update();

        SDL_Rect baseRect;
        SDL_Rect currentRect;
        SDL_Color baseColor;
        SDL_Color hoverColor;
        SDL_Color textColor;

    private:
        /*
        SDL_Rect baseRect;
        SDL_Rect currentRect;
        SDL_Color baseColor;
        SDL_Color hoverColor;
        SDL_Color textColor;
        */

        SDL_Texture* textTexture;
        SDL_Rect textRect;

        bool hovered = false;
        bool clicked = false;

        void UpdateTextPosition();
};

#endif // BUTTON_H
