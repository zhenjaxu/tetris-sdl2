#include "TextSpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "UI.h"
#include <SDL2/SDL_ttf.h>

TextSpriteComponent::TextSpriteComponent(Actor* owner, int drawOrder)
: SpriteComponent(owner, drawOrder)
{}

void TextSpriteComponent::Draw(SDL_Renderer* renderer)
{
    auto find = dynamic_cast<UI*>(mOwner);
    if(!find) return;

    auto texts = find->GetTexts();
    DrawTexts(renderer, texts);
}

void TextSpriteComponent::DrawTexts(SDL_Renderer* renderer, std::shared_ptr<std::vector<Text>> texts)
{
    TTF_Font* font = mOwner->GetGame()->GetRenderer()->GetFont();

    for(auto text : *texts)
    {
        if (text.content.empty()) return;

        SDL_Color color = {text.color.r, text.color.g, text.color.b, text.color.a};

        SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text.content.c_str(), color, 0);
        if (!surface) continue;

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture)
        {
            SDL_FreeSurface(surface);
            continue;
        }

        SDL_Rect dstRect = { text.x, text.y, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}