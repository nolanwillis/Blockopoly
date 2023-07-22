// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPGameMode.h"
#include "../BLPGameInstance.h"
#include "../State/BLPGameState.h"
#include "../State/BLPPlayerState.h"
#include "../Pawns/BLPAvatar.h"
#include "../Controllers/BLPPlayerController.h"

ABLPGameMode::ABLPGameMode()
{
	// Gets reference to BP_Avatar0
	const ConstructorHelpers::FClassFinder<AActor> BP_Avatar0(TEXT("/Game/Core/Pawns/BP_BLPAvatar0"));
	if (!BP_Avatar0.Class) return;
	AvatarClass0 = BP_Avatar0.Class;
	// Gets reference to BP_Avatar1
	const ConstructorHelpers::FClassFinder<AActor> BP_Avatar1(TEXT("/Game/Core/Pawns/BP_BLPAvatar1"));
	if (!BP_Avatar1.Class) return;
	AvatarClass1 = BP_Avatar1.Class;
	// Gets reference to BP_Avatar2
	const ConstructorHelpers::FClassFinder<AActor> BP_Avatar2(TEXT("/Game/Core/Pawns/BP_BLPAvatar2"));
	if (!BP_Avatar2.Class) return;
	AvatarClass2 = BP_Avatar2.Class;
	// Gets reference to BP_Avatar3
	const ConstructorHelpers::FClassFinder<AActor> BP_Avatar3(TEXT("/Game/Core/Pawns/BP_BLPAvatar3"));
	if (!BP_Avatar3.Class) return;
	AvatarClass3 = BP_Avatar3.Class;
    	
	if (AvatarClass0) { UE_LOG(LogTemp, Warning, TEXT("Avatar0 Pawn found"));}
	if (AvatarClass1) { UE_LOG(LogTemp, Warning, TEXT("Avatar1 Pawn found"));}
	if (AvatarClass2) { UE_LOG(LogTemp, Warning, TEXT("Avatar2 Pawn found"));}
	if (AvatarClass3) { UE_LOG(LogTemp, Warning, TEXT("Avatar3 Pawn found"));}
}

void ABLPGameMode::StartGame()
{
	UBLPGameInstance* BLPGameInstancePtr = Cast<UBLPGameInstance>(GetGameInstance());
	if (!BLPGameInstancePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameMode: BLPGameInstancePtr is null")); return; }	
	
	BLPGameInstancePtr->StartSession();
	UWorld* World = GetWorld();
	if (!World) return;
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

	Spawn(PlayerArray.Num()-1, NewPlayer);

	UBLPGameInstance* BLPGameInstancePtr = GetGameInstance<UBLPGameInstance>();
	if (!BLPGameInstancePtr) return;
	if (BLPGameInstancePtr->HasEnteredGame) UE_LOG(LogTemp, Warning, TEXT("Entered Game map"));
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

void ABLPGameMode::Spawn(const int& BLPPlayerId, APlayerController* NewPlayer) const
{
		UWorld* World = GetWorld();
    	if (!World) return;

		UBLPGameInstance* BLPGameInstancePtr = Cast<UBLPGameInstance>(GetGameInstance());
		ABLPPlayerController* BLPPlayerControllerPtr = Cast<ABLPPlayerController>(NewPlayer);

		if (!BLPGameInstancePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameMode: BLPGameInstancePtr is null")); return; }	
		if (!BLPPlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameMode: BLPPlayerControllerPtr is null")); return; }
    
    	const FActorSpawnParameters SpawnParameters;
		FVector SpawnLocation;
		TSubclassOf<ABLPAvatar> PawnToSpawn;
		ABLPAvatar* NewPawn = nullptr;

		switch (BLPPlayerId)
    	{
    	case 0:
    		if (BLPGameInstancePtr->HasEnteredGame) SpawnLocation = InitGameSpawnPoint0;
    		else SpawnLocation = InitLobbySpawnPoint0;
			PawnToSpawn = AvatarClass0;
    		break;
    	case 1:
    		if (BLPGameInstancePtr->HasEnteredGame) SpawnLocation = InitGameSpawnPoint1;
    		else SpawnLocation = InitLobbySpawnPoint1;
			PawnToSpawn = AvatarClass1;		
    		break;
    	case 2:
    		if (BLPGameInstancePtr->HasEnteredGame) SpawnLocation = InitGameSpawnPoint2;
    		else SpawnLocation = InitLobbySpawnPoint2;
    		PawnToSpawn = AvatarClass2;		
    		break;
    	case 3:
    		if (BLPGameInstancePtr->HasEnteredGame) SpawnLocation = InitGameSpawnPoint3;
    		else SpawnLocation = InitLobbySpawnPoint3;
    		PawnToSpawn = AvatarClass3;		
    		break;
    	default:
    		break;
    	}

		NewPawn = World->SpawnActor<ABLPAvatar>(PawnToSpawn, SpawnLocation, {0, 0, 0}, SpawnParameters);
	
		BLPPlayerControllerPtr->Possess(NewPawn);
}
