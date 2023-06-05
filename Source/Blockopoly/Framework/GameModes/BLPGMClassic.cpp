// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPGMClassic.h"

#include "../BLPPlayerController.h"
#include "../State/BLPGameState.h"
#include "Blockopoly/Framework/State/BLPPlayerState.h"
#include "Blockopoly/UI/BLPUWGameMenu.h"


void ABLPGMClassic::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ABLPGameState* GameStatePtr = GetGameState<ABLPGameState>();
	TArray<TObjectPtr<APlayerState>> PlayerArray = GameStatePtr->PlayerArray;

	// Tell all the player controllers to update their UI when a new player enters
	for (TObjectPtr<APlayerState> Player : PlayerArray)
	{
		ABLPPlayerState* PlayerStatePtr = Cast<ABLPPlayerState>(Player);
		if (!PlayerStatePtr) UE_LOG(LogTemp, Warning, TEXT("BLPGMClassic: PlayerStatePtr is null"));
		PlayerStatePtr->SetPlayerCount(PlayerArray.Num());
	}

	// Should also do the same on logout...
}


