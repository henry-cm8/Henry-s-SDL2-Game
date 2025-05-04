#include "Button.h"

Button::Button(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Rect rect, SDL_Color textcolor)
    : baseRect(rect), currentRect(rect)
{
   //Text to texture
   SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), textcolor);
   textTexture = SDL_CreateTextureFromSurface(renderer, surface);
   textRect.w = surface->w;
   textRect.h = surface->h;
   SDL_FreeSurface(surface);

   UpdateTextPosition();
}

void Button::Render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer,
                           hovered ? hoverColor.r : baseColor.r,
                           hovered ? hoverColor.g : baseColor.g,
                           hovered ? hoverColor.b : baseColor.b, 255);
    SDL_RenderFillRect(renderer, &currentRect);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
}

void Button::HandleEvent(SDL_Event& e)
{
    clicked = false;

    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && hovered) clicked = true;
}

void Button::Update()
{
    int x,y;
    SDL_GetMouseState(&x,&y);
    bool inside = (x >= baseRect.x && x <= baseRect.x + baseRect.w &&
                   y >= baseRect.y && y <= baseRect.y + baseRect.h);

    hovered = inside;
    //clicked = false;

    //Scaling
    if (hovered)
    {
        currentRect.w = static_cast<int>(baseRect.w * 1.1);
        currentRect.h = static_cast<int>(baseRect.h * 1.1);
        currentRect.x = baseRect.x - (currentRect.w - baseRect.w)/2;
        currentRect.y = baseRect.y - (currentRect.h - baseRect.h)/2;
    } else {
        currentRect = baseRect;
    }

    UpdateTextPosition();

    //if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && hovered) clicked = true;
}

void Button::UpdateTextPosition()
{
    textRect.x = currentRect.x + (currentRect.w - textRect.w) / 2;
    textRect.y = currentRect.y + (currentRect.h - textRect.h) / 2;
}

Button::~Button()
{
    SDL_DestroyTexture(textTexture);
}
