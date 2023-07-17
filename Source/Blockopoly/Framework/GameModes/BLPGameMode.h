// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BLPGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API ABLPGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	void StartGame();
    
protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
    	
};
