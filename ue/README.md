# TetrisUE（UE4 C++ 俄罗斯方块）

本文件夹是基于 `Src/` 中的 SDL2 俄罗斯方块实现，使用 Unreal Engine 4 C++ 重新实现的项目。

## 项目结构

```
ue/
├── Config/
│   └── DefaultEngine.ini      # 引擎默认配置，设置默认 GameMode
├── Content/                   # 空内容文件夹，UE4 项目必需
├── Source/
│   └── TetrisUE/
│       ├── TetrisUE.Build.cs
│       ├── TetrisUE.Target.cs
│       ├── TetrisUEEditor.Target.cs
│       ├── Public/
│       │   ├── TetrisTypes.h
│       │   ├── BoardActor.h
│       │   ├── PieceActor.h
│       │   ├── TetrisGameModeBase.h
│       │   └── TetrisPlayerController.h
│       └── Private/
│           ├── TetrisTypes.cpp
│           ├── BoardActor.cpp
│           ├── PieceActor.cpp
│           ├── TetrisGameModeBase.cpp
│           └── TetrisPlayerController.cpp
└── TetrisUE.uproject
```

## 与原 SDL2 版本的映射

| 原 SDL2 | UE4 |
|---------|-----|
| `Vector2` | `FIntPoint` |
| `SDL_Color` | `FColor` |
| `Game` | `ATetrisGameModeBase` |
| `Actor` | `AActor` |
| `Board` | `ABoardActor` |
| `Piece` | `APieceActor` |
| SDL 输入 | `ATetrisPlayerController` + `InputComponent` |
| SDL 渲染 | `DrawDebugSolidBox` 调试绘制 |

## 输入说明

| 按键 | 功能 |
|------|------|
| `A` | 左移 |
| `D` | 右移 |
| `S` | 软降（按住加速下落） |
| `W` | 顺时针旋转 |
| `Space` | 硬降 |
| `Esc` | 退出游戏 |

## 运行方式

1. 确保已安装 Unreal Engine 4.27（或兼容的 UE4 版本）。
2. 双击 `TetrisUE.uproject` 打开项目。
3. 在编辑器中点击 Play 即可开始游戏。

> 注意：当前实现使用 `DrawDebugSolidBox` 进行方块绘制，因此需要确保游戏视图中启用调试渲染（默认在编辑器 Play 模式下可见）。

## 代码说明

- **ATetrisGameModeBase**：游戏入口，负责生成 `ABoardActor` 与 `APieceActor`，并存储棋盘参数。
- **ABoardActor**：维护 `10x20` 的棋盘数据，提供碰撞检测、行消除、重置与绘制。
- **APieceActor**：处理当前下落方块的输入、自动下落、旋转、幽灵块计算与硬降。
- **ATetrisPlayerController**：使用 `InputComponent->BindKey` 直接绑定键盘按键，将输入事件转发给 `APieceActor`。
- **TetrisTypes**：集中定义方块类型枚举与 7 种标准形状数据。
