cmake_minimum_required(VERSION 3.10)
project(tetris)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_FILES
    Src/Main.cpp
    Src/Engine/Core/Game.cpp
    Src/Engine/Core/Actor.cpp
    Src/Engine/Core/Component.cpp
    Src/Engine/Core/InputComponent.cpp
    Src/Engine/Renderer/Renderer.cpp
    Src/Engine/Renderer/SpriteComponent.cpp
    Src/Engine/Renderer/BlockSpriteComponent.cpp
    Src/Engine/Renderer/TextSpriteComponent.cpp
    Src/Engine/Audio/AudioSystem.cpp
    Src/Engine/Utils/Math.cpp
    Src/Engine/Utils/Random.cpp
    Src/Game/Board.cpp
    Src/Game/Piece.cpp
    Src/Game/UI.cpp
)

# SDL2路径
set(SDL2_DIR D:/SDL2)
set(FMOD_DIR D:/FMOD1.09)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    Src/Engine/Core
    Src/Engine/Renderer
    Src/Engine/Audio
    Src/Engine/Utils
    Src/Game
)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_DIR}/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${SDL2_DIR}/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 SDL2_ttf)

target_include_directories(${PROJECT_NAME} PRIVATE ${FMOD_DIR}/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${FMOD_DIR}/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE fmod64)