#include "TetrisPlayerController.h"
#include "TetrisGameModeBase.h"
#include "PieceActor.h"

ATetrisPlayerController::ATetrisPlayerController()
{
}

void ATetrisPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATetrisPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindKey(EKeys::A, IE_Pressed, this, &ATetrisPlayerController::MoveLeft);
	InputComponent->BindKey(EKeys::D, IE_Pressed, this, &ATetrisPlayerController::MoveRight);
	InputComponent->BindKey(EKeys::W, IE_Pressed, this, &ATetrisPlayerController::Rotate);
	InputComponent->BindKey(EKeys::SpaceBar, IE_Pressed, this, &ATetrisPlayerController::HardDrop);
	InputComponent->BindKey(EKeys::S, IE_Pressed, this, &ATetrisPlayerController::EnableSoftDrop);
	InputComponent->BindKey(EKeys::S, IE_Released, this, &ATetrisPlayerController::DisableSoftDrop);
	InputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &ATetrisPlayerController::QuitGame);
}

void ATetrisPlayerController::MoveLeft()
{
	if (APieceActor* Piece = GetPieceActor())
	{
		Piece->MoveLeft();
	}
}

void ATetrisPlayerController::MoveRight()
{
	if (APieceActor* Piece = GetPieceActor())
	{
		Piece->MoveRight();
	}
}

void ATetrisPlayerController::Rotate()
{
	if (APieceActor* Piece = GetPieceActor())
	{
		Piece->Rotate();
	}
}

void ATetrisPlayerController::HardDrop()
{
	if (APieceActor* Piece = GetPieceActor())
	{
		Piece->HardDrop();
	}
}

void ATetrisPlayerController::EnableSoftDrop()
{
	if (APieceActor* Piece = GetPieceActor())
	{
		Piece->SetSoftDrop(true);
	}
}

void ATetrisPlayerController::DisableSoftDrop()
{
	if (APieceActor* Piece = GetPieceActor())
	{
		Piece->SetSoftDrop(false);
	}
}

void ATetrisPlayerController::QuitGame()
{
	ConsoleCommand("quit");
}

APieceActor* ATetrisPlayerController::GetPieceActor() const
{
	if (ATetrisGameModeBase* GameMode = Cast<ATetrisGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		return GameMode->GetPieceActor();
	}
	return nullptr;
}
