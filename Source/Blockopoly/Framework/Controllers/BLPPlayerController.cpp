// Fill out your copyright notice in the Description page of Project Settings.
#include "./BLPPlayerController.h"
#include "../../BLPCameraManager.h"
#include "../Pawns/BLPAvatar.h"
#include "../State/BLPGameState.h"
#include "../State/BLPPlayerState.h"
#include "../../Items/Spaces/BLPSpace.h"
#include "../../Items/Spaces/BLPChanceSpace.h"
#include "../../Items/Spaces/BLPChestSpace.h"
#include "../../Items/Spaces/BLPPropertySpace.h"
#include "../../Items/Spaces/BLPEstatePropertySpace.h"
#include "../../Items/Spaces/BLPGoToJailSpace.h"
#include "../../Items/Spaces/BLPJailSpace.h"
#include "../..//Items/Spaces/BLPTaxSpace.h"
#include "../../UI/BLPUWGameMenu.h"
#include "../../UI/BLPUWLobbyMenu.h"
#include "../../UI/BLPUWWinScreen.h"
#include "../GameModes/BLPGameMode.h"
#include "Blockopoly/Framework/BLPGameInstance.h"

#include "GameFramework/Controller.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

ABLPPlayerController::ABLPPlayerController()
{
	// Gets reference to WBP_GameMenu
	const ConstructorHelpers::FClassFinder<UUserWidget> WBP_GameMenu(TEXT("/Game/Core/UI/WBP_GameMenu"));
	if (!WBP_GameMenu.Class) return;
	GameMenuClass = WBP_GameMenu.Class;

	// Gets reference to WBP_GameMenu.
	const ConstructorHelpers::FClassFinder<UUserWidget> WBP_LobbyMenu(TEXT("/Game/Core/UI/WBP_LobbyMenu"));
	if (!WBP_LobbyMenu.Class) return;
	LobbyMenuClass = WBP_LobbyMenu.Class;

	// Gets reference to WBP_WinScreen.
	const ConstructorHelpers::FClassFinder<UUserWidget> WBP_WinScreen(TEXT("/Game/Core/UI/WBP_WinScreen"));
	if (!WBP_WinScreen.Class) return;
	WinScreenClass = WBP_WinScreen.Class;
}

void ABLPPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// Required, otherwise the camera target will sometimes default to the player pawn. 
	bAutoManageActiveCameraTarget=false;
	
	const UWorld* World = GetWorld();
	if (!World) return;
	BLPCameraManagerPtr = Cast<ABLPCameraManager>(UGameplayStatics::GetActorOfClass(World, ABLPCameraManager::StaticClass()));
	if (!BLPCameraManagerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: BLPCameraManagerPtr is null")); return; }
	AActor* Camera = BLPCameraManagerPtr->GetCamera(0);
	if (!Camera) return;
	SetViewTargetWithBlend(Camera, 0.0f);
}

void ABLPPlayerController::LoadLobbyMenu()
{
	// Create a WBP_LobbyMenu
	if (!LobbyMenuClass) return;
	LobbyMenu = CreateWidget<UBLPUWLobbyMenu>(this, LobbyMenuClass);
	if (!LobbyMenu) return;
	LobbyMenu->Setup();
	LobbyMenu->Refresh();
	UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: Load Lobby Menu called"));
}
void ABLPPlayerController::LoadGameMenu()
{
	// Create a WBP_GameMenu
	if (!GameMenuClass) return;
	GameMenu = CreateWidget<UBLPUWGameMenu>(this, GameMenuClass);
	if (!GameMenu) return;

	// Remove lobby menu
	if (LobbyMenu) LobbyMenu->Remove();

	GameMenu->Setup();
	GameMenu->RefreshPlayerList();
	UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: Load Game Menu called"));
}
void ABLPPlayerController::LoadPauseMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("LoadPauseMenu clicked!"));
	UBLPGameInstance* BLPGameInstancePtr = GetGameInstance<UBLPGameInstance>();
	if (!BLPGameInstancePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerContoller: BLPGameInstancePtr is null")); return; }
	BLPGameInstancePtr->LoadPauseMenu();
}
void ABLPPlayerController::LoadWinScreen(const FString& WinnersName)
{
	// Create a WBP_WinScreen
	if (!WinScreenClass) return;
	WinScreen = CreateWidget<UBLPUWWinScreen>(this, WinScreenClass);
	if (!WinScreen) return;

	// Remove game menu
	if (GameMenu) GameMenu->Remove();

	const FString WinnerDescription = "Congratulations to " + WinnersName;
	WinScreen->SetOwningPlayer(this);
	WinScreen->WinnerText->SetText(FText::FromString(WinnerDescription));
	WinScreen->Setup();
}

void ABLPPlayerController::Server_SetInitialTurnStatus_Implementation(ABLPPlayerState* BLPPlayerStateInPtr)
{
	if (!BLPPlayerStateInPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: BLPPlayerStatePtr is null")); return; }
	BLPPlayerStateInPtr->SetPlayerUpId(0);
}
bool ABLPPlayerController::Server_SetInitialTurnStatus_Validate(ABLPPlayerState* BLPPlayerStateInPtr){ return true; }

// Server RPC that starts the game from the lobby
void ABLPPlayerController::Server_PlayGame_Implementation(ABLPPlayerState* BLPPlayerStateInPtr, ABLPGameState* BLPGameStateInPtr)
{
	if (!BLPPlayerStateInPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerControllerLobby: BLPPlayerStateLobbyInPtr is null")); return; }
	if (!BLPGameStateInPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerControllerLobby: BLPGameStateLobbyInPtr is null")); return; }
    	
	if (!(BLPPlayerStateInPtr->GetLocalRole() == ROLE_Authority)) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerControllerLobby: Player dosen't have authority")); return; }
    
	TArray<int> ReadyStatusArray = BLPGameStateInPtr->GetReadyStatusArray();
	int Count = 0;
	for (const int& i : ReadyStatusArray) if(i) Count++;
	if (Count != BLPGameStateInPtr->PlayerArray.Num()) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerControllerLobby: All players are not ready")); return; }
	
	UWorld* World = GetWorld();
	if (!World) return;
	ABLPGameMode* BLPGameModePtr = Cast<ABLPGameMode>(World->GetAuthGameMode());
	if (!BLPGameModePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerControllerLobby: BLPGameModePtr is null")); return; }
	BLPGameModePtr->StartGame();
}
bool ABLPPlayerController::Server_PlayGame_Validate(ABLPPlayerState* BLPPlayerStateInPtr, ABLPGameState* BLPGameStateInPtr){ return true;}

// Server RPC that toggles a players ready status for in the game state
void ABLPPlayerController::Server_ToggleIsReady_Implementation(ABLPPlayerState* BLPPlayerStateInPtr, ABLPGameState* BLPGameStateInPtr)
{
	if (!BLPPlayerStateInPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: BLPPlayerStatePtr is null")); return; }
	if (!BLPGameStateInPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: BLPGameStatePtr is null")); return; }
    
	TArray<int> LocalReadyStatusArray = BLPGameStateInPtr->GetReadyStatusArray();
	const int BLPPlayerId = BLPPlayerStateInPtr->GetBLPPlayerId();
    
	if (LocalReadyStatusArray[BLPPlayerId]) LocalReadyStatusArray[BLPPlayerId] = 0;
	else LocalReadyStatusArray[BLPPlayerId] = 1;
    
	BLPGameStateInPtr->SetReadyStatusArray(LocalReadyStatusArray);
}
bool ABLPPlayerController::Server_ToggleIsReady_Validate(ABLPPlayerState* BLPPlayerStateInPtr, ABLPGameState* BLPGameStateInPtr){ return true; }

// Server RPC that simulates a dice roll
void ABLPPlayerController::Server_Roll_Implementation(ABLPAvatar* AvatarPtr, ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	if (!AvatarPtr) { UE_LOG(LogTemp, Warning, TEXT("AvatarPtr is null, from PC")); return; }
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerStatePtr is null, from PC")); return; }
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("GameStatePtr is null, from PC")); return; }

	if (!PlayerStatePtr->GetBLPPlayerId() == PlayerStatePtr->GetPlayerUpId())
	{
		UE_LOG(LogTemp, Warning, TEXT("It's not your turn!"))
		return;
	}

	if (PlayerStatePtr->GetHasRolled())
	{
		UE_LOG(LogTemp, Warning, TEXT("You've already rolled!"));
		return;
	}

	if (PlayerStatePtr->GetJailCounter() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Your in jail for %d more turns!"), PlayerStatePtr->GetJailCounter());
		return;
	}
	
	const TArray<ABLPSpace*> SpaceList = GameStatePtr->GetSpaceList();

	if (SpaceList.IsEmpty()) { UE_LOG(LogTemp, Warning, TEXT("Sent SpaceList is empty, from PC")); return; }

	const int DiceValue = FMath::RandRange(2,12);
	UE_LOG(LogTemp, Warning, TEXT("You rolled a: %d"), DiceValue);
	int NewSpaceID = PlayerStatePtr->GetCurrentSpaceId() + DiceValue;
	const int MaxSpaceID = GameStatePtr->GetSpaceList().Num()-1;
	
	// If we pass go
	if (NewSpaceID > MaxSpaceID)
	{
		NewSpaceID = NewSpaceID-MaxSpaceID-1;
		PlayerStatePtr->AddToBalance(200);
	}
	
	PlayerStatePtr->SetCurrentSpaceId(NewSpaceID);
	PlayerStatePtr->SetHasRolled(true);

	UE_LOG(LogTemp, Warning, TEXT("CurrentSpaceId is: %d"), PlayerStatePtr->GetCurrentSpaceId());

	GameStatePtr->AddRollNotificationToUI(DiceValue, PlayerStatePtr);
}
bool ABLPPlayerController::Server_Roll_Validate(ABLPAvatar* AvatarPtr, ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr){ return true; }

// Server RPC that fires when the dice roll notification is finished
void ABLPPlayerController::Server_ReflectRollInGame_Implementation(ABLPAvatar* AvatarPtr, ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: PlayerStatePtr is null")); return; }
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: GameStatePtr is null")); return; }
	if (!AvatarPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: AvatarPtr is null, ReflectRollInGame()")); return; }
     
	const TArray<ABLPSpace*> SpaceList = GameStatePtr->GetSpaceList();
	MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
	
	if (GetRemoteRole() == ROLE_AutonomousProxy)
	{
		PlayerStatePtr->Client_SimulateMoveLocally(PlayerStatePtr->GetCurrentSpaceId());
	}
	
	ApplySpaceEffect(PlayerStatePtr, GameStatePtr);
	
	PlayerStatePtr->SetHasRolled(true);
}
bool ABLPPlayerController::Server_ReflectRollInGame_Validate(ABLPAvatar* AvatarPtr, ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr){ return true; }

// Server RPC that finishes the current turn and moves to the next
void ABLPPlayerController::Server_FinishTurn_Implementation(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerStatePtr is null, from PC")); return; }
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("GameStatePtr is null, from PC")); return; }

	if (!PlayerStatePtr->GetBLPPlayerId() == PlayerStatePtr->GetPlayerUpId())
	{
		UE_LOG(LogTemp, Warning, TEXT("It's not your turn!"))
		return;
	}

	if (!PlayerStatePtr->GetHasRolled() && PlayerStatePtr->GetJailCounter() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("You need to roll before finishing your turn!"))
		return;
	}

	if (PlayerStatePtr->GetJailCounter() > 0)
	{
		PlayerStatePtr->SetJailCounter(PlayerStatePtr->GetJailCounter() - 1);
	}
	
	GameStatePtr->NextPlayerUp();
	PlayerStatePtr->SetCanBuyCurrentProperty(false);
	PlayerStatePtr->SetHasRolled(false);
}
bool ABLPPlayerController::Server_FinishTurn_Validate(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr){ return true; }

void ABLPPlayerController::Server_SkipJail_Implementation(ABLPPlayerState* PlayerStatePtr)
{
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerStatePtr is null, from PC")); return; }
    
	if (!PlayerStatePtr->GetBLPPlayerId() == PlayerStatePtr->GetPlayerUpId())
	{
		UE_LOG(LogTemp, Warning, TEXT("It's not your turn!"))
		return;
	}
	
	if (PlayerStatePtr->GetJailSkipCounter() >= 1)
	{
		PlayerStatePtr->SetJailCounter(0);
		PlayerStatePtr->SetJailSkipCounter(PlayerStatePtr->GetJailSkipCounter()-1);
		PlayerStatePtr->SetHasRolled(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("You have no get out of jail free cards!"));
	}
    	
}
bool ABLPPlayerController::Server_SkipJail_Validate(ABLPPlayerState* PlayerStatePtr){ return true; }

void ABLPPlayerController::Server_Forfeit_Implementation(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: PlayerStatePtr is null")); return; }
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: GameStatePtr is null")); return; }
	
	TArray<int> NewForfeitedPlayersArray = GameStatePtr->GetForfeitedPlayersArray();
	NewForfeitedPlayersArray.Add(PlayerStatePtr->GetBLPPlayerId());
	GameStatePtr->SetForfeitedPlayersArray(NewForfeitedPlayersArray);

	// Reset ownership of properties
	for (ABLPPropertySpace* PropertySpace : PlayerStatePtr->GetOwnedPropertyList())
	{
		PropertySpace->SetOwnerId(-1);
		GameStatePtr->AddToAvailablePropertySpaceList(PropertySpace);
	}

	GameStatePtr->AddForfeitNotificationToUI(PlayerStatePtr);
	
	GameStatePtr->NextPlayerUp();
}
bool ABLPPlayerController::Server_Forfeit_Validate(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr){ return true; }

// Server RPC that allows a user to buy a property that has no owner
void ABLPPlayerController::Server_BuyPropertySpace_Implementation(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerStatePtr is null, from PC")); return; }
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("GameStatePtr is null, from PC")); return; }
	
	if (!PlayerStatePtr->GetBLPPlayerId() == PlayerStatePtr->GetPlayerUpId())
	{
		UE_LOG(LogTemp, Warning, TEXT("It's not your turn!"))
		return;
	}
	
	ABLPPropertySpace* PropertySpaceToPurchase = Cast<ABLPPropertySpace>(GameStatePtr->GetSpaceFromId(PlayerStatePtr->GetCurrentSpaceId()));

	if (!PlayerStatePtr->GetCanBuyCurrentProperty())
	{
		UE_LOG(LogTemp, Warning, TEXT("This property is not available for purchase"));
		return;
	}

	const int RemainingBalance = PlayerStatePtr->GetBalance() - PropertySpaceToPurchase->GetPurchaseCost();
	if (RemainingBalance < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("You need %d more credits"), abs(RemainingBalance));
		return;
	}
	
	PropertySpaceToPurchase->SetOwnerId(PlayerStatePtr->GetBLPPlayerId());
	PlayerStatePtr->AddToOwnedPropertyList(PropertySpaceToPurchase);
	GameStatePtr->RemoveFromAvailablePropertySpaceList(PropertySpaceToPurchase);
	PlayerStatePtr->AddToBalance(-PropertySpaceToPurchase->GetPurchaseCost());
	PlayerStatePtr->SetCanBuyCurrentProperty(false);

	// If this is an estate property, check if player can now build on the family of estate properties.
	if (const ABLPEstatePropertySpace* EstatePropertySpace = Cast<ABLPEstatePropertySpace>(PropertySpaceToPurchase))
	{
		UpdateCanBuild(EstatePropertySpace, PlayerStatePtr);
	}
}
bool ABLPPlayerController::Server_BuyPropertySpace_Validate(ABLPPlayerState* PlayerStatePtr,ABLPGameState* GameStatePtr){ return true; }

// Server RPC that allows a user to mortgage/unmortgage a property
void ABLPPlayerController::Server_ToggleMortgageStatus_Implementation(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr, const int& SpaceID)
{
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: GameStatePtr is null")); return; }
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: PlayerStatePtr is null")); return; }
	if (SpaceID < 0) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: SpaceID invalid")); return; }
	
	if (!PlayerStatePtr->GetBLPPlayerId() == PlayerStatePtr->GetPlayerUpId())
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: It's not your turn!"))
		return;
	}
	
	ABLPPropertySpace* PropertySpacePtr = Cast<ABLPPropertySpace>(GameStatePtr->GetSpaceFromId(SpaceID));

	if (!PropertySpacePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: This is not a property!")); return; }
	
	if (PlayerStatePtr->GetPlayerId() != PropertySpacePtr->GetOwnerId())
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: You don't own this property!"))
		return;
	}

	if (PropertySpacePtr->GetIsMortgaged())
	{
		PropertySpacePtr->SetIsMortgaged(false);
		PlayerStatePtr->AddToBalance(-PropertySpacePtr->GetMortgageValue());
	}
	else
	{
		PropertySpacePtr->SetIsMortgaged(true);
		PlayerStatePtr->AddToBalance(PropertySpacePtr->GetMortgageValue());
	}
}
bool ABLPPlayerController::Server_ToggleMortgageStatus_Validate(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr, const int& SpaceID){ return true; }

// Server RPC that enables a user to build a building on a property
void ABLPPlayerController::Server_BuyBuilding_Implementation(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr, const int& SpaceID)
{
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: GameStatePtr is null")); return; }
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: PlayerStatePtr is null")); return; }
	if (SpaceID < 0) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: SpaceID invalid")); return; }
	
	if (!PlayerStatePtr->GetBLPPlayerId() == PlayerStatePtr->GetPlayerUpId())
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: It's not your turn!"))
		return;
	}
	
	ABLPEstatePropertySpace*  EstatePropertySpace = Cast<ABLPEstatePropertySpace>(GameStatePtr->GetSpaceFromId(SpaceID));

	if (!EstatePropertySpace) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: This is not an estate property!")); return; }
	
	if (PlayerStatePtr->GetPlayerId() != EstatePropertySpace->GetOwnerId())
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: You don't own this estate property!"))
		return;
	}

	if (!EstatePropertySpace->GetCanBuild())
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: You don't own all the %s estate properties "), *EstatePropertySpace->GetFamilyColor());
		return;
	}

	PlayerStatePtr->AddToBalance(-EstatePropertySpace->GetBuildingCost());
	const int UpdatedBuildingCount = EstatePropertySpace->GetBuildingCount() + 1;
	EstatePropertySpace->SetBuildingCount(UpdatedBuildingCount);
	UpdateBuildings(EstatePropertySpace, UpdatedBuildingCount);
	UpdateRent(EstatePropertySpace, UpdatedBuildingCount);
}
bool ABLPPlayerController::Server_BuyBuilding_Validate(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr,const int& SpaceID){ return true; }

// Server RPCs that invoke ExecuteCard Functions on the given player in the GameState
void ABLPPlayerController::Server_ExecuteChanceCard_Implementation(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	GameStatePtr->ExecuteChanceCard(PlayerStatePtr);
}
bool ABLPPlayerController::Server_ExecuteChanceCard_Validate(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	return true;
}
void ABLPPlayerController::Server_ExecuteChestCard_Implementation(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	GameStatePtr->ExecuteChestCard(PlayerStatePtr);
}
bool ABLPPlayerController::Server_ExecuteChestCard_Validate(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	return true;
}

// Moves player (should always be called from the server)
void ABLPPlayerController::MovePlayer(ABLPAvatar* AvatarPtr, ABLPPlayerState* PlayerStatePtr, const TArray<ABLPSpace*>& SpaceList)
{
	for (ABLPSpace* Space : SpaceList)
	{
		if (Space->GetSpaceID() == PlayerStatePtr->GetCurrentSpaceId())
		{
			FSpawnPoint* OldSpawnPoint = PlayerStatePtr->GetCurrentSpawnPoint();
			if (OldSpawnPoint) OldSpawnPoint->Taken = false;

			FSpawnPoint* NewSpawnPoint = nullptr;

			// If player is in jail get special spawn point else get normal spawn point
			if (PlayerStatePtr->GetJailCounter() == 3)
			{
				if (ABLPJailSpace* JailSpace = Cast<ABLPJailSpace>(Space)) NewSpawnPoint = JailSpace->GetOpenJailCell();
			}
			else
			{
				NewSpawnPoint = Space->GetOpenSpawnPoint();
			}
			
			if (!NewSpawnPoint)
			{
				UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: Spawn point could not be found!"));
				return;
			}
			PlayerStatePtr->SetCurrentSpawnPoint(NewSpawnPoint);
			
			UE_LOG(LogTemp, Warning, TEXT("SpawnPointId: %d, Location: %s" ), NewSpawnPoint->Index, *(NewSpawnPoint->Transform).ToString());
			const FVector NewLocation = Space->GetActorTransform().GetLocation() + NewSpawnPoint->Transform.GetLocation();
			const FRotator NewRotation = Space->GetActorTransform().GetRotation().Rotator(); 
			AvatarPtr->SetActorLocationAndRotation(NewLocation, NewRotation);

			AActor* NewCamera = BLPCameraManagerPtr->GetCamera(PlayerStatePtr->GetCurrentSpaceId()/10);
			if (!NewCamera) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: Camera not found when moving player")); return; }
			SetViewTargetWithBlend(NewCamera, 0.8f);
		}
	}
}

void ABLPPlayerController::SendToJail(ABLPPlayerState* PlayerStatePtr, const TArray<ABLPSpace*>& SpaceList)
{
	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(PlayerStatePtr->GetPawn());
	if (!AvatarPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: AvatarPtr is null")); return; }
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: PlayerStatePtr is null")); return; }
	
	PlayerStatePtr->SetJailCounter(3);
	PlayerStatePtr->SetCurrentSpaceId(10);
	MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
	PlayerStatePtr->Client_SimulateMoveLocally(10);
}

// Updates can build bool for a family of estate properties
void ABLPPlayerController::UpdateCanBuild(const ABLPEstatePropertySpace* EstatePropertySpacePtr, const ABLPPlayerState* BLPPlayerStatePtr) const
{
	TArray<ABLPPropertySpace*> OwnedPropertyList = BLPPlayerStatePtr->GetOwnedPropertyList();
	const FString TargetFamilyColor = EstatePropertySpacePtr->GetFamilyColor();
	TArray<ABLPEstatePropertySpace*> MatchingEstateProperties;

	for (ABLPPropertySpace* PropertySpace : OwnedPropertyList)
	{
		if (ABLPEstatePropertySpace* EstatePropertySpace = Cast<ABLPEstatePropertySpace>(PropertySpace))
		{
			if (EstatePropertySpace->GetFamilyColor() == TargetFamilyColor)
			{
				MatchingEstateProperties.Add(EstatePropertySpace);
			}
		}
	}
	if (TargetFamilyColor == "Brown" || TargetFamilyColor == "DarkBlue")
	{
		if (MatchingEstateProperties.Num() == 2)
		{
			for (ABLPEstatePropertySpace* EstatePropertySpace : MatchingEstateProperties )
			{
				EstatePropertySpace->SetCanBuild(true);
			}
		}
	}
	else
	{
		if (MatchingEstateProperties.Num() == 3)
		{
			for (ABLPEstatePropertySpace* EstatePropertySpace : MatchingEstateProperties )
			{
				EstatePropertySpace->SetCanBuild(true);
			}
		}
	}
}

// Updates the amount of buildings on an estate property
void ABLPPlayerController::UpdateBuildings(const ABLPEstatePropertySpace* EstatePropertySpacePtr, const int& BuildingCount) const
{
	if (!EstatePropertySpacePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: EtatePropertySpacePtr is null")); return; }
	
	if (BuildingCount == 1)
	{
		if (UStaticMeshComponent* HousePtr = EstatePropertySpacePtr->GetHouse0()) HousePtr->SetVisibility(true, false);
		UE_LOG(LogTemp, Warning, TEXT("BLPEstatePropertySpace: You've built your 1st house"));
	}
	else if (BuildingCount == 2)
	{
		if (UStaticMeshComponent* HousePtr = EstatePropertySpacePtr->GetHouse1()) HousePtr->SetVisibility(true, false);
		UE_LOG(LogTemp, Warning, TEXT("BLPEstatePropertySpace: You've built a 2nd house"));
	}
	else if (BuildingCount == 3)
	{
		if (UStaticMeshComponent* HousePtr = EstatePropertySpacePtr->GetHouse2()) HousePtr->SetVisibility(true, false);
		UE_LOG(LogTemp, Warning, TEXT("BLPEstatePropertySpace: You've built a 3rd house"));
	}
	else if (BuildingCount == 4)
	{
		if (UStaticMeshComponent* HousePtr = EstatePropertySpacePtr->GetHouse3()) HousePtr->SetVisibility(true, false);
		UE_LOG(LogTemp, Warning, TEXT("BLPEstatePropertySpace: You've built a 4th house"));
	}
	else if (BuildingCount == 5)
	{
		if (UStaticMeshComponent* HotelPtr = EstatePropertySpacePtr->GetHotel()) HotelPtr->SetVisibility(true, false);
		if (UStaticMeshComponent* HousePtr0 = EstatePropertySpacePtr->GetHouse0()) HousePtr0->SetVisibility(false, false);
		if (UStaticMeshComponent* HousePtr1 = EstatePropertySpacePtr->GetHouse1()) HousePtr1->SetVisibility(false, false);
		if (UStaticMeshComponent* HousePtr2 = EstatePropertySpacePtr->GetHouse2()) HousePtr2->SetVisibility(false, false);
		if (UStaticMeshComponent* HousePtr3 = EstatePropertySpacePtr->GetHouse3()) HousePtr3->SetVisibility(false, false);
		UE_LOG(LogTemp, Warning, TEXT("BLPEstatePropertySpace: You've built a hotel"));
	}
}

// Updates rent to match building count
void ABLPPlayerController::UpdateRent(ABLPEstatePropertySpace* EstatePropertySpacePtr, const int& BuildingCount) const
{
	if (!EstatePropertySpacePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: EtatePropertySpacePtr is null")); return; }
	if (BuildingCount < 0 || BuildingCount > 5) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: Invalid BuildingCount")); return; }
	
	switch(BuildingCount)
	{
		case 1:
			EstatePropertySpacePtr->SetCurrentRent(EstatePropertySpacePtr->GetRent1Houses());
		case 2:
			EstatePropertySpacePtr->SetCurrentRent(EstatePropertySpacePtr->GetRent2Houses());
		case 3:
			EstatePropertySpacePtr->SetCurrentRent(EstatePropertySpacePtr->GetRent3Houses());
		case 4:
			EstatePropertySpacePtr->SetCurrentRent(EstatePropertySpacePtr->GetRent4Houses());
		case 5:
			EstatePropertySpacePtr->SetCurrentRent(EstatePropertySpacePtr->GetRentHotel());
		default:
			break;
	}
}

// Applies correct side effect depending on what space is landed on
void ABLPPlayerController::ApplySpaceEffect(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: GameStatePtr is null")); return; }
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: PlayerStatePtr is null")); return; }
	
	UE_LOG(LogTemp, Warning, TEXT("ABLPPlayerController: ApplySpaceSideEffect called"));
	const int EnteredSpaceID = PlayerStatePtr->GetCurrentSpaceId();
	ABLPSpace* EnteredSpace = GameStatePtr->GetSpaceFromId(EnteredSpaceID);
	if (!EnteredSpace)
	{
		UE_LOG(LogTemp, Warning, TEXT("Entered space could not be found"));
		return;
	}
	if (Cast<ABLPPropertySpace>(EnteredSpace))
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: Property Space Entered"));
		const ABLPPropertySpace* EnteredPropertySpace = Cast<ABLPPropertySpace>(EnteredSpace);
		CheckIfPropertyIsForSale(PlayerStatePtr, GameStatePtr);
		ChargeRent(PlayerStatePtr, GameStatePtr, EnteredPropertySpace);
	}
	else if (Cast<ABLPChanceSpace>(EnteredSpace))
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: Chance Space Entered"));
		DrawChanceCard(PlayerStatePtr, GameStatePtr);
	}
	else if (Cast<ABLPChestSpace>(EnteredSpace))
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: Chest Space Entered"));
		DrawChestCard(PlayerStatePtr, GameStatePtr);
	}
	else if (Cast<ABLPGoToJailSpace>(EnteredSpace))
	{
		UE_LOG(LogTemp, Warning, TEXT("GO TO JAIL!!"));
		SendToJail(PlayerStatePtr, GameStatePtr->GetSpaceList());
	}
	else if (Cast<ABLPTaxSpace>(EnteredSpace))
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: Taxes Collected"));
		PlayerStatePtr->AddToBalance(-100);
	}
}

// Collects rent from player if they do not own the property they move to
void ABLPPlayerController::ChargeRent(ABLPPlayerState* PlayerStatePtr, const ABLPGameState* GameStatePtr, const ABLPPropertySpace* EnteredPropertySpace) const
{
	if (EnteredPropertySpace->GetOwnerId() == -1) return;

	if (EnteredPropertySpace->GetIsMortgaged()) return;
	
	if (EnteredPropertySpace->GetOwnerId() != PlayerStatePtr->GetPlayerId())
	{
		PlayerStatePtr->AddToBalance(-EnteredPropertySpace->GetCurrentRent());

		ABLPPlayerState* OwnerOfProperty = GameStatePtr->GetOwnerOfProperty(EnteredPropertySpace);
			
		if (!OwnerOfProperty) UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: Owner of property could not be found!"));

		OwnerOfProperty->AddToBalance(EnteredPropertySpace->GetCurrentRent());
	}
}

// Triggers a card to be drawn and a notifications to be sent to clients
void ABLPPlayerController::DrawChanceCard(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr) const 
{
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: GameStatePtr is null")); return; }
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: PlayerStatePtr is null")); return; }
	
	int RandomCardIndex = FMath::RandRange(0,GameStatePtr->GetMaxChanceCards()-1);
	while (RandomCardIndex == GameStatePtr->GetCurrentChanceCardIndex())
	{
		RandomCardIndex = FMath::RandRange(0, GameStatePtr->GetMaxChanceCards()-1);
	}
	GameStatePtr->SetCurrentChanceCardIndex(RandomCardIndex);

	FString Description;

	switch (RandomCardIndex)
	{
		case 0:
			Description = "Advance to Boardwalk";
			break;
		case 1:
			Description = "Advance to Go, collect $200";
			break;
		case 2:
			Description = "Advance to Illinois Avenue. If you pass Go, collect $200";
			break;
		case 3:
			Description = "Advance to St. Charles Place. If you pass Go, collect $200";
			break;
		case 4:
			Description = "Advance to the nearest Railroad. If unowned, you may buy it from the"
								"Bank. If owned, pay the owner twice the rent";
			break;
		case 5:
			Description = "Advance to the nearest Railroad. If unowned, you may buy it from the"
								"Bank. If owned, pay the owner twice the rent";
			break;
		case 6:
			Description = "Advance to the nearest Utility. If unowned, you may buy it from the"
								"Bank. If owned, throw the dice and pay the owner ten times the amount"
								"thrown";
			break;
		case 7:
			Description = "Bank pays you a dividend of $50";
			break;
		case 8:
			Description = "Get Out of Jail Free";
			break;
		case 9:
			Description = "Go back 3 Spaces";
			break;
		case 10:
			Description = "Go to Jail. Go directly to Jail, do not pass Go, do not collect $200.";
			break;
		case 11:
			Description = "Make general repairs on all your property. For each house pay $25."
							   "For each hotel pay $100.";
			break;
		case 12:
			Description = "Speeding fine $15.";
			break;
		case 13:
			Description = "Take a trip to Reading Railroad. If you pass Go, collect $200.";
			break;
		case 14:
			Description = "You have been elected Chairmen of the Board. Pay each player $50.";
			break;
		case 15:
			Description = "Your building loan matures. Collect $150.";
			break;
		default:
			break;
	}
	
	GameStatePtr->AddCardDrawNotificationToUI("Chance", Description, PlayerStatePtr);
}
void ABLPPlayerController::DrawChestCard(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr) const 
{
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: GameStatePtr is null")); return; }
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: PlayerStatePtr is null")); return; }

	int RandomCardIndex = FMath::RandRange(0, GameStatePtr->GetMaxChestCards()-1);
	while (RandomCardIndex == GameStatePtr->GetCurrentChestCardIndex())
	{
		RandomCardIndex = FMath::RandRange(0, GameStatePtr->GetMaxChestCards()-1);
	}
	GameStatePtr->SetCurrentChestCardIndex(RandomCardIndex);

	FString Description;
	
	switch (RandomCardIndex)
	{
	case 0:
		Description = "Advance to Go, collect $200";
		break;
	case 1:
		Description = "Bank error in your favor. Collect $200.";
		break;
	case 2:
		Description = "Doctor's fee. Pay $50.";
		break;
	case 3:
		Description = "Go to Jail. Go directly to jail, do not pass Go, do not collect $200.";
		break;
	case 4:
		Description = "Holiday fund matures. Receive $100.";
		break;
	case 5:
		Description = "Income tax refund. Collect $20.";
		break;
	case 6:
		Description = "It is your birthday! Collect $10 from every player.";
		break;
	case 7:
		Description = "Life insurance matures. Collect $100.";
		break;
	case 8:
		Description = "Pay hospital fees of $100.";
		break;
	case 9:
		Description = "Pay school fees of $50";
		break;
	case 10:
		Description = "Receive $25 consultancy fee.";
		break;
	case 11:
		Description = "You are assessed for street repair. $40 per house. $115 per hotel.";
		break;
	case 12:
		Description = "You have won second prize in a beauty contest. Collect $10.";
		break;
	case 13:
		Description = "You inherit $100";
		break;
	case 14:
		Description = "From sale of stock you get $50.";
		break;
	case 15:
		Description = "Get Out of Jail Free";
		break;
	default:
		break;
	}
	
	GameStatePtr->AddCardDrawNotificationToUI("Community Chest", Description, PlayerStatePtr);
}

void ABLPPlayerController::CheckIfPropertyIsForSale(ABLPPlayerState* PlayerStatePtr, const ABLPGameState* GameStatePtr) const
{
	ABLPSpace* Space = GameStatePtr->GetSpaceFromId(PlayerStatePtr->GetCurrentSpaceId());
	if (const ABLPPropertySpace* PropertySpace = Cast<ABLPPropertySpace>(Space))
	{
		if (PropertySpace->GetOwnerId() == -1)
		{
			PlayerStatePtr->SetCanBuyCurrentProperty(true);
		}
		else
		{
			PlayerStatePtr->SetCanBuyCurrentProperty(false);
		}
	}
}
