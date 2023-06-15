// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./BLPSpace.h"
#include "BLPJailSpace.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API ABLPJailSpace : public ABLPSpace
{
	GENERATED_BODY()

public:
	// Gets the transform of an open jail cell from the JailCells array
	FSpawnPoint* GetOpenJailCell();

protected:
	virtual void BeginPlay() override;

private:
	// Array of spawn points
	TArray<FSpawnPoint*> JailCells{ &JailCell0, &JailCell1, &JailCell2, &JailCell3 };

	UPROPERTY(EditAnywhere, Category = JailCells, meta = (AllowPrivateAccess = true))
	FSpawnPoint JailCell0;

	UPROPERTY(EditAnywhere, Category = JailCells, meta = (AllowPrivateAccess = true))
	FSpawnPoint JailCell1;

	UPROPERTY(EditAnywhere, Category = JailCells, meta = (AllowPrivateAccess = true))
	FSpawnPoint JailCell2;

	UPROPERTY(EditAnywhere, Category = JailCells, meta = (AllowPrivateAccess = true))
	FSpawnPoint JailCell3;
};
