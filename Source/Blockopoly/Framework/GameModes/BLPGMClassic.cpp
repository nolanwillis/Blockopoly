// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPGMClassic.h"

#include "../State/BLPGameState.h"
#include "../State/BLPPlayerState.h"

void ABLPGMClassic::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	const ABLPGameState* GameStatePtr = GetGameState<ABLPGameState>();
	
	TArray<TObjectPtr<APlayerState>> PlayerArray = GameStatePtr->PlayerArray;

	ABLPPlayerState* NewBLPPlayerStatePtr = NewPlayer->GetPlayerState<ABLPPlayerState>();
	if (!NewBLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGMClassic: BLPPlayerStatePtr is null")); return; }
	NewBLPPlayerStatePtr->SetBLPPlayerId(PlayerArray.Num()-1);

	// Tell all the player controllers to update their UI when a new player enters
	for (APlayerState* PlayerStatePtr : PlayerArray)
	{
		ABLPPlayerState* BLPPlayerStatePtr = Cast<ABLPPlayerState>(PlayerStatePtr);
		if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGMClassic: BLPPlayerStatePtr is null")); return; }
		BLPPlayerStatePtr->SetPlayerCount(PlayerArray.Num());
	}

	// Should also do the same on logout...
}


