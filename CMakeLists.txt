cmake_minimum_required(VERSION 3.10)
project(tetris_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TEST_FILES
    Test/Main.cpp
    Test/Game/Game.cpp
    Test/Game/Actor.cpp
    Test/Game/Board.cpp
    Test/Game/Piece.cpp
)

# SDL2路径
set(SDL2_DIR D:/SDL2)

add_executable(${PROJECT_NAME} ${TEST_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE Test/Game)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_DIR}/include)
target_link_directories(${PROJECT_NAME} PRIVATE ${SDL2_DIR}/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)