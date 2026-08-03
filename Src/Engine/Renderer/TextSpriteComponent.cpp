#include "TextSpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "UI.h"
#include "Shader.h"
#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>

TextSpriteComponent::TextSpriteComponent(Actor* owner, int drawOrder)
: SpriteComponent(owner, drawOrder)
{}

void TextSpriteComponent::Draw(Shader* shader)
{
    auto find = dynamic_cast<UI*>(mOwner);
    if(!find) return;

    auto texts = find->GetTexts();
    DrawTexts(shader, texts);
}

void TextSpriteComponent::DrawTexts(Shader* shader, std::shared_ptr<std::vector<Text>> texts)
{
    TTF_Font* font = mOwner->GetGame()->GetRenderer()->GetFont();

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    shader->SetBoolUniform("uUseTexture", true);

    for(const auto& text : *texts)
    {
        if (text.content.empty()) return;

        SDL_Color color = {
            static_cast<Uint8>(255 * text.color.a), 
            static_cast<Uint8>(255 * text.color.b), 
            static_cast<Uint8>(255 * text.color.g), 
            static_cast<Uint8>(255 * text.color.r)
        };
        SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text.content.c_str(), color, 0);
        if (!surface) continue;
        SDL_Surface* rgbaSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
        if (!rgbaSurface) 
        {
            SDL_FreeSurface(surface);
            continue;
        }

        // Uint32* pixels = (Uint32*)rgbaSurface->pixels;
        // Uint8 r, g, b, a;
        // SDL_GetRGBA(pixels[0], rgbaSurface->format, &r, &g, &b, &a);
        // printf("BG pixel: R=%d G=%d B=%d A=%d\n", r, g, b, a);

        // printf("surface pitch: %d, rgbaSurface pitch: %d\n", 
        //     surface->pitch, rgbaSurface->pitch);

        // printf("surface w=%d h=%d, rgbaSurface w=%d h=%d\n",
        //     surface->w, surface->h, rgbaSurface->w, rgbaSurface->h);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 
                     0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaSurface->pixels);

        // std::vector<GLubyte> gpuPixels(surface->w * surface->h * 4);
        // glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, gpuPixels.data());

        // printf("GPU first pixel: R=%d G=%d B=%d A=%d\n",
        //     gpuPixels[0], gpuPixels[1], gpuPixels[2], gpuPixels[3]);

        Matrix4 world = Matrix4::CreateScale(surface->w, surface->h, 1.0f);
        world *= Matrix4::CreateRotationZ(0.0f);
        world *= Matrix4::CreateTranslation(Vector3(text.x + surface->w / 2, text.y - surface->h / 2, 0.0f));
        shader->SetMatrixUniform("uWorldTransform", world);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        SDL_FreeSurface(rgbaSurface);
        SDL_FreeSurface(surface);
    }

    // for(auto text : *texts)
    // {
    //     if (text.content.empty()) return;

    //     SDL_Color color = {text.color.r, text.color.g, text.color.b, text.color.a};

    //     SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text.content.c_str(), color, 0);
    //     if (!surface) continue;

    //     SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    //     if (!texture)
    //     {
    //         SDL_FreeSurface(surface);
    //         continue;
    //     }

    //     SDL_Rect dstRect = { text.x, text.y, surface->w, surface->h };
    //     SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    //     SDL_DestroyTexture(texture);
    //     SDL_FreeSurface(surface);
    // }
}