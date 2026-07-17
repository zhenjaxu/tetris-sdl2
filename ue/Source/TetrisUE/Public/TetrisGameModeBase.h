#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BoardActor.h"
#include "PieceActor.h"
#include "TetrisGameModeBase.generated.h"

// 游戏主模式：负责生成棋盘与当前方块，并暴露配置参数
UCLASS()
class TETRISUE_API ATetrisGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATetrisGameModeBase();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	ABoardActor* GetBoardActor() const { return BoardActor; }
	APieceActor* GetPieceActor() const { return PieceActor; }

	int32 GetBoardColumns() const { return BoardColumns; }
	int32 GetBoardRows() const { return BoardRows; }
	int32 GetBoardCellSize() const { return BoardCellSize; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Tetris|Config")
	int32 BoardColumns;

	UPROPERTY(EditDefaultsOnly, Category = "Tetris|Config")
	int32 BoardRows;

	UPROPERTY(EditDefaultsOnly, Category = "Tetris|Config")
	int32 BoardCellSize;

	UPROPERTY()
	ABoardActor* BoardActor;

	UPROPERTY()
	APieceActor* PieceActor;
};
