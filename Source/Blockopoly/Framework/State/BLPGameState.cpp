// Fill out your copyright notice in the Description page of Project Settings.

#include "BLPGameState.h"
#include "BLPPlayerState.h"
#include "../Controllers/BLPPlayerController.h"
#include "../../Items/Spaces/BLPPropertySpace.h"
#include "../Pawns/BLPAvatar.h"
#include "../../Items/Spaces/BLPEstatePropertySpace.h"
#include "../../BLPCameraManager.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

void ABLPGameState::BeginPlay()
{
	Super::BeginPlay();
	
	const UWorld* World = GetWorld();
	if (!World) return;
	BLPCameraManagerPtr = Cast<ABLPCameraManager>(UGameplayStatics::GetActorOfClass(World, ABLPCameraManager::StaticClass()));
	if (!BLPCameraManagerPtr) UE_LOG(LogTemp, Warning, TEXT("BLPGameState: BLPCameraManagerPtr is null")); 
}

void ABLPGameState::OnRep_ReadyStatusArray()
{
	for (APlayerState* PlayerStatePtr : PlayerArray)
	{
		ABLPPlayerState* BLPPlayerStatePtr = Cast<ABLPPlayerState>(PlayerStatePtr);
		if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameStateLobby: BLPPlayerStatePtr is null")); return; }
		BLPPlayerStatePtr->Client_RefreshUI();
	}
}

void ABLPGameState::WinnersPlayerIdCallback()
{
	const ABLPPlayerState* WinnersPlayerStatePtr = GetBLPPlayerStateFromId(WinnersPlayerId);
	if (!WinnersPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: WinnersPlayerStatePtr is null")); return; }
	
	// Add win screen to all clients
	for (APlayerState* PlayerStatePtr : PlayerArray)
	{
		ABLPPlayerState* BLPPlayerStatePtr = Cast<ABLPPlayerState>(PlayerStatePtr);
		if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: BLPPlayerStatePtr is null")); return; }
		BLPPlayerStatePtr->Client_DisplayWinScreen(WinnersPlayerStatePtr->GetPlayerName());
    }	
}

void ABLPGameState::CheckForWinner()
{
	// Check for win condition (all players forfeit except 1)
	if (ForfeitedPlayersArray.Num() == PlayerArray.Num()-1)
	{
		for (APlayerState* PlayerStatePtr : PlayerArray)
		{
			const ABLPPlayerState* BLPPlayerStatePtr = Cast<ABLPPlayerState>(PlayerStatePtr);
			if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: BLPPlayerStatePtr is null")); return; }

			const int Id = BLPPlayerStatePtr->GetBLPPlayerId();
    			
			if (!ForfeitedPlayersArray.Contains(Id))
			{
				UE_LOG(LogTemp, Warning, TEXT("Winner declared in BLPGameState!"));
				SetWinnersPlayerId(Id);
				break;
			}	
		}
	}
}

void ABLPGameState::OnRep_ForfeitedPlayersArray()
{
	for (APlayerState* PlayerStatePtr : PlayerArray)
	{
		ABLPPlayerState* BLPPlayerStatePtr = Cast<ABLPPlayerState>(PlayerStatePtr);
		if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: BLPPlayerStatePtr is null")); return; }
		BLPPlayerStatePtr->Client_RefreshUI();
	}
	
	CheckForWinner();
}

// Returns the player state of the owner of a given property
ABLPPlayerState* ABLPGameState::GetOwnerOfProperty(const ABLPPropertySpace* EnteredPropertySpace) const
{
	for (APlayerState* PlayerStatePtr : PlayerArray)
	{
		ABLPPlayerState* BLPPlayerStatePtr = Cast<ABLPPlayerState>(PlayerStatePtr);
		if (BLPPlayerStatePtr->GetPlayerId() == EnteredPropertySpace->GetOwnerId())
		{
			return Cast<ABLPPlayerState>(BLPPlayerStatePtr);
		}
	}
	return nullptr;
}

ABLPSpace* ABLPGameState::GetSpaceFromId(const int& ID) const
{
	for (ABLPSpace* Space : SpaceList)
	{
		if (Space->GetSpaceID() == ID) return Space;
	}
	UE_LOG(LogTemp, Warning, TEXT("BLPGameState: Could not find space with given ID"));
	return nullptr;
}

ABLPPlayerState* ABLPGameState::GetBLPPlayerStateFromId(const int& ID) const
{
	for (APlayerState* PlayerStatePtr : PlayerArray)
	{
		if (ABLPPlayerState* BLPPlayerStatePtr = Cast<ABLPPlayerState>(PlayerStatePtr))
		{
			if (BLPPlayerStatePtr->GetBLPPlayerId() == ID) return BLPPlayerStatePtr;
		}
	}
	return nullptr;
}

void ABLPGameState::NextPlayerUp() 
{
	const int PlayerCount = PlayerArray.Num();
	
	if (GSPlayerUpId == PlayerCount-1)
	{
		GSPlayerUpId = 0;
	}
	else
	{
		GSPlayerUpId += 1;
	}

	for (APlayerState* PlayerStatePtr : PlayerArray)
	{
		ABLPPlayerState* BLPPlayerStatePtr = Cast<ABLPPlayerState>(PlayerStatePtr);
		if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: BLPPlayerStatePtr is null")); return; }
		BLPPlayerStatePtr->SetPlayerUpId(GSPlayerUpId);
	}
}

void ABLPGameState::ExecuteChanceCard(ABLPPlayerState* PlayerStatePtr)
{
	if (CurrentChanceCardIndex < 0 || CurrentChanceCardIndex >= MaxChanceCards) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: CurrentChanceCardIndex out of range!")); return; }
	const BLPGameStateFuncPtr CardFunctionPtr = ChanceCards[CurrentChanceCardIndex];
	(this->*CardFunctionPtr)(PlayerStatePtr);
}
void ABLPGameState::ExecuteChestCard(ABLPPlayerState* PlayerStatePtr)
{
	if (CurrentChestCardIndex < 0 || CurrentChestCardIndex >= MaxChestCards) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: CurrentChestCardIndex out of range!")); return; }
	const BLPGameStateFuncPtr CardFunctionPtr = ChestCards[CurrentChestCardIndex];
	(this->*CardFunctionPtr)(PlayerStatePtr);
}

// Chance Card Functions
void ABLPGameState::ChanceCard0(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Advance to Boardwalk"));

	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(PlayerStatePtr->GetPlayerController());
	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(PlayerStatePtr->GetPawn());

	if (!AvatarPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: AvatarPtr is null")); return; }
	if (!PlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: PlayerControllerPtr is null")); return; }
	
	PlayerStatePtr->SetCurrentSpaceId(39);

	PlayerControllerPtr->MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
	PlayerStatePtr->Client_SimulateMoveLocally(39);
	
	PlayerControllerPtr->ApplySpaceEffect(PlayerStatePtr, this);
}
void ABLPGameState::ChanceCard1(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Advance to Go, collect $200"))

	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(PlayerStatePtr->GetPlayerController());
	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(PlayerStatePtr->GetPawn());

	if (!AvatarPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: AvatarPtr is null")); return; }
	if (!PlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: PlayerControllerPtr is null")); return; }

	// This is required since were not using the RollDice function which normally takes care of passing Go.
	PlayerStatePtr->AddToBalance(200);
	
	PlayerStatePtr->SetCurrentSpaceId(0);
	
	PlayerControllerPtr->MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
	PlayerStatePtr->Client_SimulateMoveLocally(0);
}
void ABLPGameState::ChanceCard2(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Advance to Illinois Avenue. If you pass Go, collect $200"));
	
	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(PlayerStatePtr->GetPlayerController());
	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(PlayerStatePtr->GetPawn());

	if (!AvatarPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: AvatarPtr is null")); return; }
	if (!PlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: PlayerControllerPtr is null")); return; }

	// This is required since were not using the RollDice function which normally takes care of passing Go.
	if (PlayerStatePtr->GetCurrentSpaceId() > 24)
	{
		PlayerStatePtr->AddToBalance(200);
	}
		
	PlayerStatePtr->SetCurrentSpaceId(24);

	PlayerControllerPtr->MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
	PlayerStatePtr->Client_SimulateMoveLocally(24);
	
	PlayerControllerPtr->ApplySpaceEffect(PlayerStatePtr, this);
}
void ABLPGameState::ChanceCard3(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Advance to St. Charles Place. If you pass Go, collect $200"));

	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(PlayerStatePtr->GetPlayerController());
	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(PlayerStatePtr->GetPawn());

	if (!AvatarPtr){ UE_LOG(LogTemp, Warning, TEXT("BLPGameState: AvatarPtr is null")); return; }
	if (!PlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: PlayerControllerPtr is null")); return; }

	// This is required since were not using the RollDice function which normally takes care of passing Go.
	if (PlayerStatePtr->GetCurrentSpaceId() > 11)
	{
		PlayerStatePtr->AddToBalance(200);
	}
	
	PlayerStatePtr->SetCurrentSpaceId(11);
	
	PlayerControllerPtr->MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
	PlayerStatePtr->Client_SimulateMoveLocally(11);
	
	PlayerControllerPtr->ApplySpaceEffect(PlayerStatePtr, this);
}
void ABLPGameState::ChanceCard4(ABLPPlayerState* PlayerStatePtr)
{
	// ApplySpaceEffect not setting can buy to true if unowned.
	UE_LOG(LogTemp, Warning, TEXT("Advance to the nearest Railroad. If unowned, you may buy it from the "
								"Bank. If owned, pay the owner twice the rent"));
	
	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(PlayerStatePtr->GetPlayerController());
	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(PlayerStatePtr->GetPawn());

	if (!AvatarPtr){ UE_LOG(LogTemp, Warning, TEXT("BLPGameState: AvatarPtr is null")); return; }
	if (!PlayerControllerPtr){ UE_LOG(LogTemp, Warning, TEXT("BLPGameState: PlayerControllerPtr is null")); return; }

	const int CurrentSpaceID = PlayerStatePtr->GetCurrentSpaceId();
	if (CurrentSpaceID == 7)
	{
		PlayerStatePtr->SetCurrentSpaceId(15);
		PlayerStatePtr->Client_SimulateMoveLocally(15);
	}
	else if (CurrentSpaceID == 22)
	{
		PlayerStatePtr->SetCurrentSpaceId(25);
		PlayerStatePtr->Client_SimulateMoveLocally(25);
	}
	else
	{
		PlayerStatePtr->SetCurrentSpaceId(5);
		PlayerStatePtr->Client_SimulateMoveLocally(5);
	}
	PlayerControllerPtr->MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);

	const ABLPPropertySpace* CurrentRailroad =  Cast<ABLPPropertySpace>(GetSpaceFromId(PlayerStatePtr->GetCurrentSpaceId()));

	if (!CurrentRailroad){ UE_LOG(LogTemp, Warning, TEXT("BLPGameState: Current railroad is null")); return; }
	
	ABLPPlayerState* OwningPlayer = GetOwnerOfProperty(CurrentRailroad);
	
	if (OwningPlayer)
	{
		// If property is owned charge double
		PlayerStatePtr->AddToBalance(-200);
		OwningPlayer->AddToBalance(200);
	}
	
	PlayerControllerPtr->ApplySpaceEffect(PlayerStatePtr, this);
}
void ABLPGameState::ChanceCard5(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Advance to the nearest Utility. If unowned, you may buy it from the"
								"Bank. If owned, throw the dice and pay the owner ten times the amount"
								"thrown"));
	
	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(PlayerStatePtr->GetPlayerController());
	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(PlayerStatePtr->GetPawn());

	if (!AvatarPtr){ UE_LOG(LogTemp, Warning, TEXT("BLPGameState: AvatarPtr is null")); return; }
	if (!PlayerControllerPtr){ UE_LOG(LogTemp, Warning, TEXT("BLPGameState: PlayerControllerPtr is null")); return; }

	const int CurrentSpaceID = PlayerStatePtr->GetCurrentSpaceId();
	if (CurrentSpaceID <= 7)
	{
		PlayerStatePtr->SetCurrentSpaceId(12);
		PlayerStatePtr->Client_SimulateMoveLocally(12);
	}
	else if (CurrentSpaceID < 22 && CurrentSpaceID > 7)
	{
		PlayerStatePtr->SetCurrentSpaceId(28);
		PlayerStatePtr->Client_SimulateMoveLocally(15);
	}
	else
	{
		PlayerStatePtr->SetCurrentSpaceId(12);
		PlayerStatePtr->Client_SimulateMoveLocally(15);
	}
	PlayerControllerPtr->MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
	
	const ABLPPropertySpace* CurrentUtility = Cast<ABLPPropertySpace>(GetSpaceFromId(PlayerStatePtr->GetCurrentSpaceId()));
	
	if (!CurrentUtility){ UE_LOG(LogTemp, Warning, TEXT("BLPGameState: Current utility is null")); return; }
	
	ABLPPlayerState* OwningPlayer = GetOwnerOfProperty(CurrentUtility);
	
	if (OwningPlayer)
	{
		const int DiceRoll = FMath::RandRange(2, 12);
		const int AmountPaid = DiceRoll * 9;
		UE_LOG(LogTemp, Warning, TEXT("You rolled a %d, so you owe %d"), DiceRoll, AmountPaid);
		PlayerStatePtr->AddToBalance(-AmountPaid);
		OwningPlayer->AddToBalance(AmountPaid);
	}
	else
	{
		PlayerControllerPtr->CheckIfPropertyIsForSale(PlayerStatePtr, this);
	}
}
void ABLPGameState::ChanceCard6(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Bank pays you a dividend of $50"));

	PlayerStatePtr->AddToBalance(50);
}
void ABLPGameState::ChanceCard7(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Get Out of Jail Free"));

	const int CurrentJailSkipCount = PlayerStatePtr->GetJailSkipCounter();
	if (CurrentJailSkipCount < 2)
	{
		PlayerStatePtr->SetJailSkipCounter(CurrentJailSkipCount+1);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("You already have 2 of these! So you get nothing!"));
	}
}
void ABLPGameState::ChanceCard8(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Go back 3 Spaces"));
	
	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(PlayerStatePtr->GetPlayerController());
	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(PlayerStatePtr->GetPawn());

	if (!AvatarPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: AvatarPtr is null")); return; }
	if (!PlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: PlayerControllerPtr is null")); return; }
	
	const int CurrentSpaceId = PlayerStatePtr->GetCurrentSpaceId();
	const int NewSpaceId = CurrentSpaceId - 3;
	PlayerStatePtr->SetCurrentSpaceId(NewSpaceId);

	PlayerControllerPtr->MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
	PlayerStatePtr->Client_SimulateMoveLocally(NewSpaceId);

	PlayerControllerPtr->ApplySpaceEffect(PlayerStatePtr, this);
}
void ABLPGameState::ChanceCard9(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Go to Jail. Go directly to Jail, do not pass Go, do not collect $200."));

	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(PlayerStatePtr->GetPlayerController());
	
	if (!PlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: PlayerControllerPtr is null")); return; }

	PlayerControllerPtr->SendToJail(PlayerStatePtr, SpaceList);
}
void ABLPGameState::ChanceCard10(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Make general repairs on all your property. For each house pay $25."
							   "For each hotel pay $100."));
	
	TArray<ABLPPropertySpace*> OwnedPropertyList = PlayerStatePtr->GetOwnedPropertyList();
	int HotelCount = 0;
	int HouseCount = 0;
	for (ABLPPropertySpace* Property : OwnedPropertyList)
	{
		if (const ABLPEstatePropertySpace* EstateProperty = Cast<ABLPEstatePropertySpace>(Property))
		{
			const int BuildingCount = EstateProperty->GetBuildingCount();
			if (BuildingCount < 5)
			{
				HouseCount += BuildingCount;
			}
			else
			{
				HotelCount += 1;
			}
		}
	}
	PlayerStatePtr->AddToBalance((HouseCount*-25) + (HotelCount*-100));
}
void ABLPGameState::ChanceCard11(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Speeding fine $15."));

	PlayerStatePtr->AddToBalance(-15);
}
void ABLPGameState::ChanceCard12(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Take a trip to Reading Railroad. If you pass Go, collect $200."));

	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(PlayerStatePtr->GetPlayerController());
	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(PlayerStatePtr->GetPawn());

	if (!AvatarPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: AvatarPtr is null")); return; }
	if (!PlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: PlayerControllerPtr is null")); return; }

	// This is required since were not using the RollDice function which normally takes care of passing Go.
	if (PlayerStatePtr->GetCurrentSpaceId() > 5)
	{
		PlayerStatePtr->AddToBalance(200);
	}
		
	PlayerStatePtr->SetCurrentSpaceId(5);
	PlayerControllerPtr->MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
	PlayerStatePtr->Client_SimulateMoveLocally(5);
	PlayerControllerPtr->ApplySpaceEffect(PlayerStatePtr, this);
	
}
void ABLPGameState::ChanceCard13(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("You have been elected Chairmen of the Board. Pay each player $50."));

	const int PlayerCount = PlayerArray.Num();
	for (int i = 0; i < PlayerCount; i++)
	{
		if (i != GSPlayerUpId)
		{
			ABLPPlayerState* OtherBLPPlayerStatePtr = Cast<ABLPPlayerState>(GetBLPPlayerStateFromId(i));
			OtherBLPPlayerStatePtr->AddToBalance(50);
		}
	}
	PlayerStatePtr->AddToBalance((PlayerCount-1) * -50);
}
void ABLPGameState::ChanceCard14(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Your building loan matures. Collect $150."));

	PlayerStatePtr->AddToBalance(150);
}

// Chest Card Functions
void ABLPGameState::ChestCard0(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Advance to Go, collect $200"));

	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(PlayerStatePtr->GetPlayerController());
	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(PlayerStatePtr->GetPawn());

	if (!AvatarPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: AvatarPtr is null")); return; }
	if (!PlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: PlayerControllerPtr is null")); return; }

	// This is required since were not using the RollDice function which normally takes care of passing Go.
	PlayerStatePtr->AddToBalance(200);
	
	PlayerStatePtr->SetCurrentSpaceId(0);

	PlayerControllerPtr->MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
	PlayerStatePtr->Client_SimulateMoveLocally(0);
}
void ABLPGameState::ChestCard1(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Bank error in your favor. Collect $200."));

	PlayerStatePtr->AddToBalance(200);
}
void ABLPGameState::ChestCard2(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Doctor's fee. Pay $50."));

	PlayerStatePtr->AddToBalance(-50);
}
void ABLPGameState::ChestCard3(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Go to Jail. Go directly to jail, do not pass Go, do not collect $200."));

	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(PlayerStatePtr->GetPlayerController());
	
	if (!PlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: PlayerControllerPtr is null")); return; }

	PlayerControllerPtr->SendToJail(PlayerStatePtr, SpaceList);
}
void ABLPGameState::ChestCard4(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Holiday fund matures. Receive $100."));

	PlayerStatePtr->AddToBalance(100);
}
void ABLPGameState::ChestCard5(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Income tax refund. Collect $20."));

	PlayerStatePtr->AddToBalance(20);
}
void ABLPGameState::ChestCard6(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("It is your birthday! Collect $10 from every player."));

	const int PlayerCount = PlayerArray.Num();
	for (int i = 0; i < PlayerCount; i++)
	{
		if (i != GSPlayerUpId)
		{
			ABLPPlayerState* OtherBLPPlayerStatePtr = Cast<ABLPPlayerState>(GetBLPPlayerStateFromId(i));
			OtherBLPPlayerStatePtr->AddToBalance(-10);
		}
	}
	
	PlayerStatePtr->AddToBalance((PlayerCount-1) * 10);
}
void ABLPGameState::ChestCard7(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Life insurance matures. Collect $100."));

	PlayerStatePtr->AddToBalance(100);
}
void ABLPGameState::ChestCard8(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Pay hospital fees of $100."));

	PlayerStatePtr->AddToBalance(-100);
}
void ABLPGameState::ChestCard9(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Pay school fees of $50"));

	PlayerStatePtr->AddToBalance(50);
}
void ABLPGameState::ChestCard10(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Receive $25 consultancy fee."));

	PlayerStatePtr->AddToBalance(-25);
}
void ABLPGameState::ChestCard11(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("You are assessed for street repair. $40 per house. $115 per hotel."))

	TArray<ABLPPropertySpace*> OwnedPropertyList = PlayerStatePtr->GetOwnedPropertyList();
	int HotelCount = 0;
	int HouseCount = 0;
	for (ABLPPropertySpace* Property : OwnedPropertyList)
	{
		if (const ABLPEstatePropertySpace* EstateProperty = Cast<ABLPEstatePropertySpace>(Property))
		{
			const int BuildingCount = EstateProperty->GetBuildingCount();
			if (BuildingCount < 5)
			{
				HouseCount += BuildingCount;
			}
			else
			{
				HotelCount += 1;
			}
		}
	}
	PlayerStatePtr->AddToBalance((HouseCount*-40) + (HotelCount*-115));
}
void ABLPGameState::ChestCard12(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("You have won second prize in a beauty contest. Collect $10."));

	PlayerStatePtr->AddToBalance(10);
}
void ABLPGameState::ChestCard13(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("You inherit $100"));

	PlayerStatePtr->AddToBalance(100);
}
void ABLPGameState::ChestCard14(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("From sale of stock you get $50."));

	PlayerStatePtr->AddToBalance(50);
}
void ABLPGameState::ChestCard15(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Get Out of Jail Free"));

	const int CurrentJailSkipCount = PlayerStatePtr->GetJailSkipCounter();
	if (CurrentJailSkipCount < 2)
	{
		PlayerStatePtr->SetJailSkipCounter(CurrentJailSkipCount+1);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("You already have 2 of these! So you get nothing!"));
	}
}


// UI Notification Functions
void ABLPGameState::AddCardDrawNotificationToUI(const FString& Type, const FString& Description, const ABLPPlayerState* BLPPlayerStateInPtr)
{
	const FString Heading = BLPPlayerStateInPtr->GetPlayerName() + " Took a " + Type + " Card";
	
	for (TObjectPtr<APlayerState> PlayerState : PlayerArray)
	{
		ABLPPlayerState* BLPPlayerStatePtr = Cast<ABLPPlayerState>(PlayerState);
		if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: BLPPlayerStatePtr is null")); return; }
		BLPPlayerStatePtr->Client_AddNotification(Type, Heading, Description);
	}
}

void ABLPGameState::AddRollNotificationToUI(const int& Number, const ABLPPlayerState* BLPPlayerStateInPtr)
{
	const FString Heading = BLPPlayerStateInPtr->GetPlayerName() + " Rolled a ";
	
	for (TObjectPtr<APlayerState> PlayerState : PlayerArray)
	{
		ABLPPlayerState* BLPPlayerStatePtr = Cast<ABLPPlayerState>(PlayerState);
		if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: BLPPlayerStatePtr is null")); return; }
		BLPPlayerStatePtr->Client_AddNotification("Roll", Heading, FString::FromInt(Number));
	}
}

void ABLPGameState::AddForfeitNotificationToUI(const ABLPPlayerState* BLPPlayerStateInPtr)
{
	const FString Description = BLPPlayerStateInPtr->GetPlayerName() + " has forfeit";
    	
	for (TObjectPtr<APlayerState> PlayerState : PlayerArray)
	{
		ABLPPlayerState* BLPPlayerStatePtr = Cast<ABLPPlayerState>(PlayerState);
		if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: BLPPlayerStatePtr is null")); return; }
		BLPPlayerStatePtr->Client_AddNotification("Forfeit", "", Description);
	}
}

void ABLPGameState::AddLeaveNotificationToUI(const ABLPPlayerState* BLPPlayerStateInPtr)
{
	const FString Description = BLPPlayerStateInPtr->GetPlayerName() + " has left the game";
	for (TObjectPtr<APlayerState> PlayerState : PlayerArray)
	{
		ABLPPlayerState* BLPPlayerStatePtr = Cast<ABLPPlayerState>(PlayerState);
		if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: BLPPlayerStatePtr is null")); return; }

		BLPPlayerStatePtr->Client_AddNotification("Leave", "", Description);
	}
}

void ABLPGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Here we list the variables we want to replicate
	DOREPLIFETIME(ABLPGameState, CurrentChanceCardIndex);
	DOREPLIFETIME(ABLPGameState, CurrentChestCardIndex);
	DOREPLIFETIME(ABLPGameState, ReadyStatusArray);
	DOREPLIFETIME(ABLPGameState, AvailablePropertySpaceList);
	DOREPLIFETIME(ABLPGameState, ForfeitedPlayersArray);
}
