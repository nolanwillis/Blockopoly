// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPGMClassic.h"

#include "../State/BLPGameState.h"
#include "../State/BLPPlayerState.h"

void ABLPGMClassic::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	const ABLPGameState* BLPGameStatePtr = GetGameState<ABLPGameState>();
	ABLPPlayerState* BLPPlayerStatePtr = NewPlayer->GetPlayerState<ABLPPlayerState>();
	if (!BLPGameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGMClassic: BLPGameStatePtr is null")); return; }
	if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGMClassic: BLPPlayerStatePtr is null")); return; }

	TArray<TObjectPtr<APlayerState>> PlayerArray = BLPGameStatePtr->PlayerArray;

	BLPPlayerStatePtr->SetBLPPlayerId(PlayerArray.Num()-1);

	// Tell all the player controllers to update their UI when a new player enters
	for (APlayerState* PlayerState : PlayerArray)
	{
		ABLPPlayerState* BLPPlayerState = Cast<ABLPPlayerState>(PlayerState);
		if (!BLPPlayerState) { UE_LOG(LogTemp, Warning, TEXT("BLPGMClassic: BLPPlayerStatePtr is null")); return; }
		BLPPlayerState->SetPlayerCount(PlayerArray.Num());
	}
}


