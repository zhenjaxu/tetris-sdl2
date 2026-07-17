#include "BoardActor.h"
#include "DrawDebugHelpers.h"

ABoardActor::ABoardActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Columns = 10;
	Rows = 20;
	CellSize = 30;

	// I, T, O, L, J, S, Z 七种方块对应颜色
	Colors = {
		FColor(0, 255, 255),    // I - Cyan
		FColor(128, 0, 128),    // T - Purple
		FColor(255, 255, 0),    // O - Yellow
		FColor(255, 165, 0),    // L - Orange
		FColor(0, 0, 255),       // J - Blue
		FColor(0, 255, 0),       // S - Green
		FColor(255, 0, 0)        // Z - Red
	};
}

void ABoardActor::BeginPlay()
{
	Super::BeginPlay();
	Reset();
}

void ABoardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 绘制已锁定方块
	for (int32 Y = 0; Y < Rows; ++Y)
	{
		for (int32 X = 0; X < Columns; ++X)
		{
			if (Grid[Y][X] == -1) continue;

			const FColor& Color = Colors[Grid[Y][X]];
			FVector Center, Extent;
			GetCellBounds(X, Y, Center, Extent);
			DrawDebugSolidBox(GetWorld(), Center, Extent, Color, FRotator::ZeroRotator, false, -1.0f, 0);
		}
	}
}

bool ABoardActor::IsValid(const FIntPoint Blocks[4]) const
{
	for (int32 i = 0; i < 4; ++i)
	{
		// 检查左右边界与下边界；上边界留出生成区域，暂不检查
		if (Blocks[i].X < 0 || Blocks[i].X >= Columns || Blocks[i].Y >= Rows)
		{
			return false;
		}
		// 只检查已进入可视区域的格子是否已被占用
		if (Blocks[i].Y >= 0 && Grid[Blocks[i].Y][Blocks[i].X] != -1)
		{
			return false;
		}
	}
	return true;
}

void ABoardActor::Lock(const FIntPoint Blocks[4], ETetrisPieceType Type)
{
	int32 TypeIndex = static_cast<int32>(Type);
	for (int32 i = 0; i < 4; ++i)
	{
		// 忽略未进入可视区域的部分，避免数组越界
		if (Blocks[i].Y >= 0)
		{
			Grid[Blocks[i].Y][Blocks[i].X] = TypeIndex;
		}
	}
}

void ABoardActor::ClearLines()
{
	// 从底部向上扫描，发现满行则删除并在顶部插入空行
	for (int32 Y = Rows - 1; Y >= 0; --Y)
	{
		bool bFull = true;
		for (int32 X = 0; X < Columns; ++X)
		{
			if (Grid[Y][X] == -1)
			{
				bFull = false;
				break;
			}
		}
		if (bFull)
		{
			Grid.RemoveAt(Y);
			Grid.Insert(TArray<int32>(Columns, -1), 0);
			++Y; // 消除后需要再次检查当前行
		}
	}
}

void ABoardActor::Reset()
{
	// 使用 -1 表示空格子，重新分配二维棋盘
	Grid.Empty(Rows);
	for (int32 Y = 0; Y < Rows; ++Y)
	{
		Grid.Add(TArray<int32>(Columns, -1));
	}
}

void ABoardActor::SetBoardParameters(int32 InColumns, int32 InRows, int32 InCellSize)
{
	Columns = InColumns;
	Rows = InRows;
	CellSize = InCellSize;
	Reset();
}

void ABoardActor::GetCellBounds(int32 X, int32 Y, FVector& OutCenter, FVector& OutExtent) const
{
	OutCenter = FVector(
		X * CellSize + CellSize * 0.5f,
		-Y * CellSize - CellSize * 0.5f,
		0.0f
	);
	OutExtent = FVector(CellSize * 0.45f, CellSize * 0.45f, CellSize * 0.45f);
}
