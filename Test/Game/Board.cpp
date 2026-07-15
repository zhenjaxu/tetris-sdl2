#include"Board.h"
#include"Game.h"

// 初始化棋盘尺寸、7 种方块颜色，并清空棋盘
Board::Board(Game* game, int columns, int rows, int cell)
:Actor(game)
,mColumns(columns)
,mRows(rows)
,mCell(cell)
,mColors{
    // I, T, O, L, J, S, Z 七种方块对应颜色
    {0,255,255,255}, {128,0,128,255}, {255,255,0,255},
    {255,165,0,255}, {0,0,255,255}, {0,255,0,255}, {255,0,0,255}
}
{
    Reset();
}

void Board::Reset(){
    // 使用 -1 表示空格子，重新分配二维棋盘
    mGrid.assign(mRows, std::vector<int>(mColumns, -1));
}

bool Board::IsValid(const Vector2 blocks[4]) const {
    for(int i=0;i<4;++i){
        // 检查左右边界与下边界；上边界留出生成区域，暂不检查
        if(blocks[i].x<0||
           blocks[i].x>=mColumns||
           blocks[i].y>=mRows) return false;
        // 只检查已进入可视区域的格子是否已被占用
        if(blocks[i].y>=0&&
           mGrid[blocks[i].y][blocks[i].x]!=-1) return false;
    }
    return true;
}

void Board::Lock(const Vector2 blocks[4], int type){
    for(int i=0;i<4;++i){
        // 忽略未进入可视区域的部分，避免数组越界
        if(blocks[i].y>=0) mGrid[blocks[i].y][blocks[i].x]=type;
    }
}

void Board::ClearLines(){
    // 从底部向上扫描，发现满行则删除并在顶部插入空行
    for(int y=mRows-1;y>=0;--y){
        bool full=true;
        for(int x=0;x<mColumns;++x){
            if(mGrid[y][x]==-1){
                full=false;
                break;
            }
        }
        if(full){
            mGrid.erase(mGrid.begin()+y);
            mGrid.insert(mGrid.begin(), std::vector<int>(mColumns, -1));
            ++y; // 消除后需要再次检查当前行
        }
    }
}

void Board::Draw(SDL_Renderer* renderer){
    for(int y=0;y<mRows;++y){
        for(int x=0;x<mColumns;++x){
            if(mGrid[y][x]==-1) continue;
            const SDL_Color& c=mColors[mGrid[y][x]];
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
            SDL_Rect rc={x*mCell+1, y*mCell+1, mCell-2, mCell-2};
            SDL_RenderFillRect(renderer, &rc);
        }
    }
}