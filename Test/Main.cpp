#include"Game/Game.h"
#undef main  // SDL 在 Windows 下会重定义 main，需取消以使用标准入口

int main(int argc, char** argv){
    Game game;
    bool success=game.Initialize();
    if(success){
        game.RunLoop();
    }
    game.Shutdown();
    return 0;
}