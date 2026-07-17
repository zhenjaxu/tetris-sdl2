#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardActor.h"
#include "TetrisTypes.h"
#include "PieceActor.generated.h"

// 当前下落方块 Actor：处理输入、自动下落、旋转、幽灵块与硬降
UCLASS()
class TETRISUE_API APieceActor : public AActor
{
	GENERATED_BODY()

public:
	APieceActor();

	virtual void Tick(float DeltaTime) override;

	// 输入接口
	void MoveLeft();
	void MoveRight();
	void Rotate();
	void HardDrop();
	void SetSoftDrop(bool bEnabled);

	// 初始化：传入棋盘引用
	void Initialize(ABoardActor* InBoardActor);

protected:
	virtual void BeginPlay() override;

private:
	// 随机生成新方块
	void Spawn();

	// 计算方块落到底部的投影
	void CalculateGhost(FIntPoint OutGhost[4]) const;

	// 顺时针旋转
	void RotateBlocks(FIntPoint OutBlocks[4]) const;

	// 绘制实体块与幽灵块
	void DrawPiece();

	// 计算指定格子在世界空间中的调试绘制中心和半尺寸
	void GetCellBounds(int32 X, int32 Y, FVector& OutCenter, FVector& OutExtent) const;

	UPROPERTY()
	ABoardActor* BoardActor;

	FIntPoint Blocks[4];
	FIntPoint Ghost[4];
	FIntPoint Position;
	ETetrisPieceType Type;

	// 下落速度配置
	float Speed;
	float DropSpeed;
	float DropAccumulate;
};
