#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TetrisTypes.h"
#include "BoardActor.generated.h"

// 棋盘 Actor：维护已锁定方块、碰撞检测、消行与绘制
UCLASS()
class TETRISUE_API ABoardActor : public AActor
{
	GENERATED_BODY()

public:
	ABoardActor();

	void Tick(float DeltaTime) override;

	// 碰撞检测：是否越界或与已固定方块重叠
	bool IsValid(const FIntPoint Blocks[4]) const;

	// 将当前方块锁定到棋盘上
	void Lock(const FIntPoint Blocks[4], ETetrisPieceType Type);

	// 消除已满的行
	void ClearLines();

	// 清空棋盘
	void Reset();

	int32 GetColumns() const { return Columns; }
	int32 GetRows() const { return Rows; }
	int32 GetCellSize() const { return CellSize; }
	const TArray<FColor>& GetColors() const { return Colors; }

	// 设置棋盘参数（通常在 GameMode 中调用）
	void SetBoardParameters(int32 InColumns, int32 InRows, int32 InCellSize);

protected:
	void BeginPlay() override;

private:
	// 计算指定格子在世界空间中的调试绘制中心和半尺寸
	void GetCellBounds(int32 X, int32 Y, FVector& OutCenter, FVector& OutExtent) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tetris|Board", meta = (AllowPrivateAccess = "true"))
	int32 Columns;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tetris|Board", meta = (AllowPrivateAccess = "true"))
	int32 Rows;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tetris|Board", meta = (AllowPrivateAccess = "true"))
	int32 CellSize;

	// 棋盘数据：-1 表示空，其他数字对应方块类型索引
	TArray<TArray<int32>> Grid;

	// 七种方块颜色：I, T, O, L, J, S, Z
	TArray<FColor> Colors;
};
