// Fill out your copyright notice in the Description page of Project Settings.

#include "BLPGameState.h"
#include "BLPPlayerState.h"
#include "../BLPPlayerController.h"
#include "../../Items/Spaces/BLPPropertySpace.h"
#include "Blockopoly/Framework/Pawns/BLPAvatar.h"
#include "Blockopoly/Items/Spaces/BLPEstatePropertySpace.h"

#include "Net/UnrealNetwork.h"

void ABLPGameState::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		// Set first turn to first player to connect to the game
		ABLPPlayerState* FirstPlayer = Cast<ABLPPlayerState>(PlayerArray[0]);
		FirstPlayer->SetIsItMyTurn(true);
	}
}

// Returns the player state of the owner of a given property
ABLPPlayerState* ABLPGameState::GetOwnerOfProperty(const ABLPPropertySpace* EnteredPropertySpace) const
{
	for (TObjectPtr<APlayerState> const PlayerState : PlayerArray)
	{
		if (PlayerState->GetPlayerId() == EnteredPropertySpace->GetOwnerID())
		{
			return Cast<ABLPPlayerState>(PlayerState);
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

void ABLPGameState::DrawChanceCard(ABLPPlayerState* PlayerStatePtr)
{
	BLPGameStateFuncPtr DrawnCard = PreviousChanceCard;

	while (DrawnCard == PreviousChanceCard)
	{
		DrawnCard = ChanceCards[FMath::RandRange(0, GNum_ChanceCards-1)];
		
	}
	
	PreviousChanceCard = DrawnCard;
	(this->*DrawnCard)(PlayerStatePtr);
	
 }
void ABLPGameState::DrawChestCard(ABLPPlayerState* PlayerStatePtr)
{
	BLPGameStateFuncPtr DrawnCard = PreviousChestCard;

	while (DrawnCard == PreviousChestCard)
	{
		DrawnCard = ChestCards[FMath::RandRange(0, GNum_ChestCards-1)];
		
	}
	
	PreviousChestCard = DrawnCard;
	(this->*DrawnCard)(PlayerStatePtr);
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
	PlayerControllerPtr->ApplySpaceSideEffect(PlayerStatePtr, this);
}
void ABLPGameState::ChanceCard1(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Advance to Go, collect $200"));

	const ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(PlayerStatePtr->GetPlayerController());
	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(PlayerStatePtr->GetPawn());

	if (!AvatarPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: AvatarPtr is null")); return; }
	if (!PlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPGameState: PlayerControllerPtr is null")); return; }

	// This is required since were not using the RollDice function which normally takes care of passing Go.
	PlayerStatePtr->AddToBalance(200);
	
	PlayerStatePtr->SetCurrentSpaceId(0);
	PlayerControllerPtr->MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
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
	PlayerControllerPtr->ApplySpaceSideEffect(PlayerStatePtr, this);
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
	PlayerControllerPtr->ApplySpaceSideEffect(PlayerStatePtr, this);
}
void ABLPGameState::ChanceCard4(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Advance to the nearest Railroad. If unowned, you may buy it from the"
								"Bank. If owned, pay the owner twice the rent"));
	
	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(PlayerStatePtr->GetPlayerController());
	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(PlayerStatePtr->GetPawn());

	if (!AvatarPtr){ UE_LOG(LogTemp, Warning, TEXT("BLPGameState: AvatarPtr is null")); return; }
	if (!PlayerControllerPtr){ UE_LOG(LogTemp, Warning, TEXT("BLPGameState: PlayerControllerPtr is null")); return; }

	const int CurrentSpaceID = PlayerStatePtr->GetCurrentSpaceId();
	if (CurrentSpaceID == 7)
	{
		PlayerStatePtr->SetCurrentSpaceId(15);
	}
	else if (CurrentSpaceID == 22)
	{
		PlayerStatePtr->SetCurrentSpaceId(25);
	}
	else
	{
		PlayerStatePtr->SetCurrentSpaceId(5);
	}
	
	PlayerControllerPtr->MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
	
	const ABLPPropertySpace* CurrentRailroad = Cast<ABLPPropertySpace>(GetSpaceFromId(PlayerStatePtr->GetCurrentSpaceId()));
	ABLPPlayerState* OwningPlayer = GetOwnerOfProperty(CurrentRailroad);
	
	if (!CurrentRailroad){ UE_LOG(LogTemp, Warning, TEXT("BLPGameState: Current railroad is null")); return; }
	if (!OwningPlayer){ UE_LOG(LogTemp, Warning, TEXT("BLPGameState: Current railroad has no owner")); return; }

	// Will apply the standard rent
	PlayerControllerPtr->ApplySpaceSideEffect(PlayerStatePtr, this);

	// Charges rent twice (railroads have fixed rent)
	PlayerStatePtr->AddToBalance(-200);
	OwningPlayer->AddToBalance(200);
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
	}
	else if (CurrentSpaceID < 22 && CurrentSpaceID > 7)
	{
		PlayerStatePtr->SetCurrentSpaceId(28);
	}
	else
	{
		PlayerStatePtr->SetCurrentSpaceId(12);
	}
	
	PlayerControllerPtr->MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
	
	const ABLPPropertySpace* CurrentUtility = Cast<ABLPPropertySpace>(GetSpaceFromId(PlayerStatePtr->GetCurrentSpaceId()));
	ABLPPlayerState* OwningPlayer = GetOwnerOfProperty(CurrentUtility);
	
	if (!CurrentUtility){ UE_LOG(LogTemp, Warning, TEXT("BLPGameState: Current utility is null")); return; }
	if (!OwningPlayer){ UE_LOG(LogTemp, Warning, TEXT("BLPGameState: Current utility has no owner")); return; }

	const int DiceRoll = FMath::RandRange(2, 12);
	const int AmountPaid = DiceRoll * 10;
	UE_LOG(LogTemp, Warning, TEXT("You rolled a %d, so you owe %d"), DiceRoll, AmountPaid);

	PlayerStatePtr->AddToBalance(-AmountPaid);
	OwningPlayer->AddToBalance(AmountPaid);
	
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
	PlayerStatePtr->SetCurrentSpaceId(CurrentSpaceId-3);
	PlayerControllerPtr->MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
	PlayerControllerPtr->ApplySpaceSideEffect(PlayerStatePtr, this);
}
void ABLPGameState::ChanceCard9(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Go to Jail. Go directly to Jail, do not pass Go, do not collect $200."));

	const ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(PlayerStatePtr->GetPlayerController());
	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(PlayerStatePtr->GetPawn());

	if (!AvatarPtr) UE_LOG(LogTemp, Warning, TEXT("BLPGameState: AvatarPtr is null"));
	if (!PlayerControllerPtr) UE_LOG(LogTemp, Warning, TEXT("BLPGameState: PlayerControllerPtr is null"));

	PlayerControllerPtr->SendToJail(AvatarPtr, PlayerStatePtr, SpaceList);
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
	PlayerControllerPtr->ApplySpaceSideEffect(PlayerStatePtr, this);
	
}
void ABLPGameState::ChanceCard13(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("You have been elected Chairment of the Board. Pay each player $50."));

	const int PlayerCount = PlayerArray.Num();
	for (int i = 0; i < PlayerCount; i++)
	{
		if (i == PlayerUpIndex) continue;
		ABLPPlayerState* OtherPlayerStatePtr = Cast<ABLPPlayerState>(PlayerArray[i]);
		OtherPlayerStatePtr->AddToBalance(50);
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
	UE_LOG(LogTemp, Warning, TEXT("ChestCard0 has been drawn and initialized!"));
}
void ABLPGameState::ChestCard1(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChestCard1 has been drawn and initialized!"));
}
void ABLPGameState::ChestCard2(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChestCard2 has been drawn and initialized!"));
}
void ABLPGameState::ChestCard3(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChestCard3 has been drawn and initialized!"));
}
void ABLPGameState::ChestCard4(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChestCard4 has been drawn and initialized!"));
}
void ABLPGameState::ChestCard5(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChestCard5 has been drawn and initialized!"));
}
void ABLPGameState::ChestCard6(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChestCard6 has been drawn and initialized!"));
}
void ABLPGameState::ChestCard7(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChestCard7 has been drawn and initialized!"));
}
void ABLPGameState::ChestCard8(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChestCard8 has been drawn and initialized!"));
}
void ABLPGameState::ChestCard9(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChestCard9 has been drawn and initialized!"));
}
void ABLPGameState::ChestCard10(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChestCard10 has been drawn and initialized!"));
}
void ABLPGameState::ChestCard11(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChestCard11 has been drawn and initialized!"));
}
void ABLPGameState::ChestCard12(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChestCard12 has been drawn and initialized!"));
}
void ABLPGameState::ChestCard13(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChestCard13 has been drawn and initialized!"));
}
void ABLPGameState::ChestCard14(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChestCard14 has been drawn and initialized!"));
}
void ABLPGameState::ChestCard15(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChestCard15 has been drawn and initialized!"));
}

void ABLPGameState::OnRep_AvailablePropertySpaces()
{
	UE_LOG(LogTemp, Warning, TEXT("Available Property List Updated!"));
}

void ABLPGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Here we list the variables we want to replicate
	DOREPLIFETIME(ABLPGameState, AvailablePropertySpaceList);

}
