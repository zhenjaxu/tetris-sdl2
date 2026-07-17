#pragma once

#include "CoreMinimal.h"

#include "TetrisTypes.generated.h"

UENUM(BlueprintType)
enum class ETetrisPieceType : uint8
{
	I	UMETA(DisplayName = "I"),
	T	UMETA(DisplayName = "T"),
	O	UMETA(DisplayName = "O"),
	L	UMETA(DisplayName = "L"),
	J	UMETA(DisplayName = "J"),
	S	UMETA(DisplayName = "S"),
	Z	UMETA(DisplayName = "Z")
};

// 方块形状数据：顺序 I, T, O, L, J, S, Z
struct TETRISUE_API FTetrisShape
{
	static const FIntPoint Shapes[7][4];
};
