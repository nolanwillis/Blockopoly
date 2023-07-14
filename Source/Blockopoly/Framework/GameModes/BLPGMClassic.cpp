// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPGMClassic.h"

#include "../State/BLPGameState.h"
#include "../State/BLPPlayerState.h"
#include "../BLPGameInstance.h"

void ABLPGMClassic::StartGame()
{
	UBLPGameInstance* GameInstance = Cast<UBLPGameInstance>(GetGameInstance());
	if (!GameInstance) return;
	
	GameInstance->StartSession();
	UWorld* World = GetWorld();
	if (!World) return;
	// Enables seamless travel (connects players transition map first, then to target map)
	bUseSeamlessTravel = true;
	// Tells all player controllers connected to travel to a different map as a listen server 
	World->ServerTravel("/Game/Maps/LVL_Game");
}

void ABLPGMClassic::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UWorld* World = GetWorld();
	if (!World) return; 
	
	const ABLPGameState* BLPGameStatePtr = GetGameState<ABLPGameState>();
	ABLPPlayerState* BLPPlayerStatePtr = NewPlayer->GetPlayerState<ABLPPlayerState>();
	if (!BLPGameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGMClassic: BLPGameStatePtr is null")); return; }
	if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGMClassic: BLPPlayerStatePtr is null")); return; }

	TArray<TObjectPtr<APlayerState>> PlayerArray = BLPGameStatePtr->PlayerArray;

	BLPPlayerStatePtr->SetBLPPlayerId(PlayerArray.Num()-1);

	// Tell all the player states to update their UI when a new player enters
	for (APlayerState* PlayerState : PlayerArray)
	{
		ABLPPlayerState* BLPPlayerState = Cast<ABLPPlayerState>(PlayerState);
		if (!BLPPlayerState) { UE_LOG(LogTemp, Warning, TEXT("BLPGMClassic: BLPPlayerStatePtr is null")); return; }
		BLPPlayerState->SetPlayerCount(PlayerArray.Num());
	}
}

void ABLPGMClassic::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}


