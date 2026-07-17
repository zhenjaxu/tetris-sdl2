#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TetrisPlayerController.generated.h"

// 玩家控制器：绑定键盘输入并转发给 PieceActor
UCLASS()
class TETRISUE_API ATetrisPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATetrisPlayerController();

	virtual void SetupInputComponent() override;

protected:
	virtual void BeginPlay() override;

private:
	void MoveLeft();
	void MoveRight();
	void Rotate();
	void HardDrop();
	void EnableSoftDrop();
	void DisableSoftDrop();
	void QuitGame();

	class APieceActor* GetPieceActor() const;
};
