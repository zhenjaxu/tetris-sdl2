#include "PieceActor.h"
#include "DrawDebugHelpers.h"

APieceActor::APieceActor()
{
	PrimaryActorTick.bCanEverTick = true;

	BoardActor = nullptr;
	Position = FIntPoint(0, 0);
	Type = ETetrisPieceType::I;

	Speed = 0.5f;
	DropSpeed = 0.5f;
	DropAccumulate = 0.0f;
}

void APieceActor::BeginPlay()
{
	Super::BeginPlay();
}

void APieceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 累计时间，达到下落间隔后向下移动一格
	DropAccumulate += DeltaTime;
	if (DropAccumulate >= DropSpeed)
	{
		DropAccumulate -= DropSpeed;

		FIntPoint Next[4];
		for (int32 i = 0; i < 4; ++i)
		{
			Next[i] = FIntPoint(Blocks[i].X, Blocks[i].Y + 1);
		}

		if (BoardActor && BoardActor->IsValid(Next))
		{
			for (int32 i = 0; i < 4; ++i)
			{
				Blocks[i] = Next[i];
			}
			Position.Y++;
		}
		else
		{
			// 无法下落则锁定、消行并生成新方块
			if (BoardActor)
			{
				BoardActor->Lock(Blocks, Type);
				BoardActor->ClearLines();
				Spawn();
				// 若新方块生成即碰撞，说明游戏结束，重置棋盘
				if (!BoardActor->IsValid(Blocks))
				{
					BoardActor->Reset();
					Spawn();
				}
			}
		}
	}

	DrawPiece();
}

void APieceActor::Initialize(ABoardActor* InBoardActor)
{
	BoardActor = InBoardActor;
	Spawn();
}

void APieceActor::MoveLeft()
{
	if (!BoardActor) return;

	FIntPoint Next[4];
	for (int32 i = 0; i < 4; ++i)
	{
		Next[i] = FIntPoint(Blocks[i].X - 1, Blocks[i].Y);
	}

	if (BoardActor->IsValid(Next))
	{
		Position.X--;
		for (int32 i = 0; i < 4; ++i)
		{
			Blocks[i] = Next[i];
		}
		CalculateGhost(Ghost);
	}
}

void APieceActor::MoveRight()
{
	if (!BoardActor) return;

	FIntPoint Next[4];
	for (int32 i = 0; i < 4; ++i)
	{
		Next[i] = FIntPoint(Blocks[i].X + 1, Blocks[i].Y);
	}

	if (BoardActor->IsValid(Next))
	{
		Position.X++;
		for (int32 i = 0; i < 4; ++i)
		{
			Blocks[i] = Next[i];
		}
		CalculateGhost(Ghost);
	}
}

void APieceActor::Rotate()
{
	if (!BoardActor) return;

	FIntPoint Next[4];
	RotateBlocks(Next);

	if (BoardActor->IsValid(Next))
	{
		for (int32 i = 0; i < 4; ++i)
		{
			Blocks[i] = Next[i];
		}
		CalculateGhost(Ghost);
	}
}

void APieceActor::HardDrop()
{
	if (!BoardActor) return;

	// 直接将方块移动到幽灵块位置，锁定并检查消行
	for (int32 i = 0; i < 4; ++i)
	{
		Blocks[i] = Ghost[i];
	}

	BoardActor->Lock(Blocks, Type);
	BoardActor->ClearLines();
	Spawn();
}

void APieceActor::SetSoftDrop(bool bEnabled)
{
	DropSpeed = bEnabled ? 0.05f : Speed;
}

void APieceActor::Spawn()
{
	if (!BoardActor) return;

	Type = static_cast<ETetrisPieceType>(FMath::RandRange(0, 6));
	Position.X = BoardActor->GetColumns() / 2;
	Position.Y = 1;

	for (int32 i = 0; i < 4; ++i)
	{
		const FIntPoint& ShapeBlock = FTetrisShape::Shapes[static_cast<int32>(Type)][i];
		Blocks[i].X = ShapeBlock.X + Position.X;
		Blocks[i].Y = ShapeBlock.Y + Position.Y;
	}

	CalculateGhost(Ghost);
}

void APieceActor::CalculateGhost(FIntPoint OutGhost[4]) const
{
	if (!BoardActor) return;

	for (int32 i = 0; i < 4; ++i)
	{
		OutGhost[i] = Blocks[i];
	}

	while (true)
	{
		FIntPoint Next[4];
		for (int32 i = 0; i < 4; ++i)
		{
			Next[i] = FIntPoint(OutGhost[i].X, OutGhost[i].Y + 1);
		}

		if (!BoardActor->IsValid(Next)) break;

		for (int32 i = 0; i < 4; ++i)
		{
			OutGhost[i] = Next[i];
		}
	}
}

void APieceActor::RotateBlocks(FIntPoint OutBlocks[4]) const
{
	// 以 Position 为中心顺时针旋转 90°：
	// (x, y) 相对中心 -> (y, -x)，再加回中心坐标
	for (int32 i = 0; i < 4; ++i)
	{
		int32 RelativeX = Blocks[i].X - Position.X;
		int32 RelativeY = Blocks[i].Y - Position.Y;
		OutBlocks[i].X = Position.X + RelativeY;
		OutBlocks[i].Y = Position.Y - RelativeX;
	}
}

void APieceActor::DrawPiece()
{
	if (!BoardActor) return;

	const FColor& Color = BoardActor->GetColors()[static_cast<int32>(Type)];

	// 绘制幽灵块（投影）
	for (int32 i = 0; i < 4; ++i)
	{
		if (Ghost[i].Y < 0) continue;

		FColor GhostColor = Color;
		GhostColor.R = FMath::Clamp(Color.R / 2, 0, 255);
		GhostColor.G = FMath::Clamp(Color.G / 2, 0, 255);
		GhostColor.B = FMath::Clamp(Color.B / 2, 0, 255);
		GhostColor.A = 80;

		FVector Center, Extent;
		GetCellBounds(Ghost[i].X, Ghost[i].Y, Center, Extent);
		DrawDebugSolidBox(GetWorld(), Center, Extent, GhostColor, FRotator::ZeroRotator, false, -1.0f, 0);
	}

	// 绘制实体块
	for (int32 i = 0; i < 4; ++i)
	{
		if (Blocks[i].Y < 0) continue;

		FVector Center, Extent;
		GetCellBounds(Blocks[i].X, Blocks[i].Y, Center, Extent);
		DrawDebugSolidBox(GetWorld(), Center, Extent, Color, FRotator::ZeroRotator, false, -1.0f, 0);
	}
}

void APieceActor::GetCellBounds(int32 X, int32 Y, FVector& OutCenter, FVector& OutExtent) const
{
	int32 CellSize = BoardActor->GetCellSize();
	OutCenter = FVector(
		X * CellSize + CellSize * 0.5f,
		-Y * CellSize - CellSize * 0.5f,
		0.0f
	);
	OutExtent = FVector(CellSize * 0.45f, CellSize * 0.45f, CellSize * 0.45f);
}
