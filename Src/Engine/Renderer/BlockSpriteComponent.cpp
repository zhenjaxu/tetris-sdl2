#include "BlockSpriteComponent.h"
#include "Actor.h"
#include "Board.h"
#include "Piece.h"
#include "Shader.h"
#include <GL/glew.h>

BlockSpriteComponent::BlockSpriteComponent(Actor* owner, int drawOrder)
: SpriteComponent(owner, drawOrder)
{}

void BlockSpriteComponent::Draw(Shader* shader)
{
    auto board = dynamic_cast<Board*>(mOwner);
    auto piece = dynamic_cast<Piece*>(mOwner);
    if(!board && !piece) return;

    std::shared_ptr<std::vector<Block>> blocks;
    if(board) blocks = board->GetBlocks();
    else if(piece) blocks = piece->GetBlocks();
    DrawBlocks(shader, blocks);
}

void BlockSpriteComponent::DrawBlocks(Shader* shader, const std::shared_ptr<std::vector<Block>>& blocks)
{
    shader->SetBoolUniform("uUseTexture", false);

    for(const auto& block : *blocks)
    {
        Matrix4 world = Matrix4::CreateScale(block.w, block.h, 1.0f);
        world *= Matrix4::CreateRotationZ(0.0f);
        world *= Matrix4::CreateTranslation(Vector3(block.x + block.w / 2, block.y + block.h / 2, 0.0f));
        shader->SetMatrixUniform("uWorldTransform", world);

        if (block.blend) 
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        } 
        else glDisable(GL_BLEND);

        Vector3 color{block.color.r, block.color.g, block.color.b};
        shader->SetVectorUniform("uColor", color, block.color.a);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    // for(auto block: *blocks)
    // {
    //     if(block.blend) SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    //     else SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    //     SDL_SetRenderDrawColor(renderer, block.color.r, block.color.g, block.color.b, block.color.a);

    //     SDL_Rect rc = {
    //         block.x,
    //         block.y,
    //         block.w,
    //         block.h
    //     };

    //     SDL_RenderFillRect(renderer, &rc);
    // }
}