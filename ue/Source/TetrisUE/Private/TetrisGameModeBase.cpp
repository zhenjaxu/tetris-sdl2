#include "TetrisGameModeBase.h"
#include "TetrisPlayerController.h"

ATetrisGameModeBase::ATetrisGameModeBase()
{
	BoardColumns = 10;
	BoardRows = 20;
	BoardCellSize = 30;

	BoardActor = nullptr;
	PieceActor = nullptr;

	// 使用自定义 PlayerController 处理输入
	PlayerControllerClass = ATetrisPlayerController::StaticClass();
}

void ATetrisGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	BoardActor = GetWorld()->SpawnActor<ABoardActor>(ABoardActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (BoardActor)
	{
		BoardActor->SetBoardParameters(BoardColumns, BoardRows, BoardCellSize);
	}

	PieceActor = GetWorld()->SpawnActor<APieceActor>(APieceActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (PieceActor)
	{
		PieceActor->Initialize(BoardActor);
	}
}

void ATetrisGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
