#include "BlockSpriteComponent.h"
#include "Actor.h"
#include "Board.h"
#include "Piece.h"
#include "Config.h"
#include <SDL2/SDL.h>

BlockSpriteComponent::BlockSpriteComponent(Actor* owner, int drawOrder)
: SpriteComponent(owner, drawOrder)
{}

void BlockSpriteComponent::Draw(SDL_Renderer* renderer)
{
    auto board = dynamic_cast<Board*>(mOwner);
    auto piece = dynamic_cast<Piece*>(mOwner);
    if(!board && !piece) return;

    std::shared_ptr<std::vector<Block>> blocks;
    if(board) blocks = board->GetBlocks();
    else if(piece) blocks = piece->GetBlocks();
    DrawBlocks(renderer, blocks);
}

void BlockSpriteComponent::DrawBlocks(SDL_Renderer* renderer, const std::shared_ptr<std::vector<Block>>& blocks)
{
    for(auto block: *blocks)
    {
        if(block.blend) SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        else SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

        SDL_SetRenderDrawColor(renderer, block.color.r, block.color.g, block.color.b, block.color.a);

        SDL_Rect rc = {
            block.x,
            block.y,
            block.w,
            block.h
        };

        SDL_RenderFillRect(renderer, &rc);
    }
}