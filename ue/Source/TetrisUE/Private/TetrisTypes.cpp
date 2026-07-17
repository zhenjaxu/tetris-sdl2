#include "TetrisTypes.h"

// 七种俄罗斯方块的标准形状（相对于方块中心的局部坐标）
const FIntPoint FTetrisShape::Shapes[7][4] = {
	{FIntPoint(0, -1), FIntPoint(0, 0), FIntPoint(0, 1), FIntPoint(0, 2)},   // I
	{FIntPoint(-1, 0), FIntPoint(0, 0), FIntPoint(1, 0), FIntPoint(0, 1)},    // T
	{FIntPoint(0, 0), FIntPoint(1, 0), FIntPoint(0, 1), FIntPoint(1, 1)},     // O
	{FIntPoint(0, -1), FIntPoint(0, 0), FIntPoint(0, 1), FIntPoint(1, 1)},    // L
	{FIntPoint(0, -1), FIntPoint(0, 0), FIntPoint(0, 1), FIntPoint(-1, 1)},   // J
	{FIntPoint(1, 0), FIntPoint(0, 0), FIntPoint(0, 1), FIntPoint(-1, 1)},    // S
	{FIntPoint(-1, 0), FIntPoint(0, 0), FIntPoint(0, 1), FIntPoint(1, 1)}     // Z
};
