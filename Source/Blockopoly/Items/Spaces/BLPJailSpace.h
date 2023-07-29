// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./BLPSpace.h"
#include "BLPJailSpace.generated.h"

class USceneComponent;

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API ABLPJailSpace : public ABLPSpace
{
	GENERATED_BODY()

public:
	ABLPJailSpace();
	// Gets the transform of an open jail cell from the JailCells array
	USceneComponent* GetJailCell(const int& BLPPlayerId);

private:
	UPROPERTY(EditAnywhere, Category = Spawning, meta = (AllowPrivateAccess = true, MakeEditWidget = true))
	USceneComponent* JailCell0;
	UPROPERTY(EditAnywhere, Category = Spawning, meta = (AllowPrivateAccess = true, MakeEditWidget = true))
	USceneComponent* JailCell1;
	UPROPERTY(EditAnywhere, Category = Spawning, meta = (AllowPrivateAccess = true, MakeEditWidget = true))
	USceneComponent* JailCell2;
	UPROPERTY(EditAnywhere, Category = Spawning, meta = (AllowPrivateAccess = true, MakeEditWidget = true))
	USceneComponent* JailCell3;

	TArray<USceneComponent*> JailCells{ JailCell0, JailCell1, JailCell2, JailCell3 };
};
