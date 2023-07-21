// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPGameMode.h"
#include "../BLPGameInstance.h"
#include "../State/BLPGameState.h"
#include "../State/BLPPlayerState.h"

void ABLPGameMode::StartGame()
{
	UBLPGameInstance* BLPGameInstancePtr = Cast<UBLPGameInstance>(GetGameInstance());
	if (!BLPGameInstancePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameMode: BLPGameInstancePtr is null")); return; }	
	
	BLPGameInstancePtr->StartSession();
	UWorld* World = GetWorld();
	if (!World) return;
	// Enables seamless travel (connects players transition map first, then to target map)
	//bUseSeamlessTravel = true;
	// Tells all player controllers connected to travel to a different map as a listen server 
	World->ServerTravel("/Game/Maps/LVL_Game");
}

void ABLPGameMode::PostLogin(APlayerController* NewPlayer)
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

void ABLPGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	ABLPGameState* BLPGameStatePtr = GetGameState<ABLPGameState>();
	ABLPPlayerState* PlayerStateOfLeaver = Exiting->GetPlayerState<ABLPPlayerState>();  
	if (!BLPGameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGMClassic: BLPGameStatePtr is null")); return; }
	if (!PlayerStateOfLeaver) { UE_LOG(LogTemp, Warning, TEXT("BLPGMClassic: PlayerStateOfLeaver is null")); return; }
	
	TArray<TObjectPtr<APlayerState>> PlayerArray = BLPGameStatePtr->PlayerArray;

	if (PlayerStateOfLeaver->GetBLPPlayerId() == 0) return;

	PlayerStateOfLeaver->SetIsLeaving(true);
	
	// Tell all the player states to update their UI when a player exits
	for (APlayerState* PlayerState : PlayerArray)
	{
		ABLPPlayerState* BLPPlayerStatePtr = Cast<ABLPPlayerState>(PlayerState);
		if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGMClassic: BLPPlayerStatePtr is null")); return; }
		BLPPlayerStatePtr->SetPlayerCount(PlayerArray.Num()-1);
		const FString Description = PlayerStateOfLeaver->GetPlayerName() + " has left the game";
		BLPPlayerStatePtr->Client_AddNotification("Leave", "", Description);
	}

	// Remove Id from forfeited players array to ensure win condition can still be met
	TArray<int> NewForfeitedPlayersArray = BLPGameStatePtr->GetForfeitedPlayersArray();
	NewForfeitedPlayersArray.Remove(PlayerStateOfLeaver->GetBLPPlayerId());
	BLPGameStatePtr->SetForfeitedPlayersArray(NewForfeitedPlayersArray);

	// Check if there's only one player left, if there is they win
	if (PlayerArray.Num() == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Winner declared in BLPGameMode 82!"));
		const ABLPPlayerState* WinnersPlayerStatePtr = Cast<ABLPPlayerState>(PlayerArray[0]);
		BLPGameStatePtr->SetWinnersPlayerId(WinnersPlayerStatePtr->GetBLPPlayerId());
	}
}
