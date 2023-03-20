// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BLPGameMode.h"
#include "BLPGMLobbyGamemode.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API ABLPGMLobbyGamemode : public ABLPGameMode
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

private:
	FTimerHandle GameStartTimer;
	uint8 PlayerCount = 0;

	void StartGame();
};
