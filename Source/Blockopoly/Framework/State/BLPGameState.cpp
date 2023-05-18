// Fill out your copyright notice in the Description page of Project Settings.

#include "BLPGameState.h"
#include "BLPPlayerState.h"
#include "../BLPPlayerController.h"
#include "../../Items/Spaces/BLPPropertySpace.h"
#include "Blockopoly/Framework/Pawns/BLPAvatar.h"

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

// TODO: Put all spaced on board then test the first 5 chance cards 
// Chance Card Functions
void ABLPGameState::ChanceCard0(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Advance to Boardwalk"));

	const ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(PlayerStatePtr->GetPlayerController());
	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(PlayerStatePtr->GetPawn());

	if (!AvatarPtr) UE_LOG(LogTemp, Warning, TEXT("BLPGameState: AvatarPtr is null"));
	if (!PlayerControllerPtr) UE_LOG(LogTemp, Warning, TEXT("BLPGameState: PlayerControllerPtr is null"));
	
	PlayerStatePtr->SetCurrentSpaceId(39);
	PlayerControllerPtr->MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
}
void ABLPGameState::ChanceCard1(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Advance to Go, collect $200"));

	const ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(PlayerStatePtr->GetPlayerController());
	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(PlayerStatePtr->GetPawn());

	if (!AvatarPtr) UE_LOG(LogTemp, Warning, TEXT("BLPGameState: AvatarPtr is null"));
	if (!PlayerControllerPtr) UE_LOG(LogTemp, Warning, TEXT("BLPGameState: PlayerControllerPtr is null"));

	// This is required since were not using the RollDice function which normally takes care of passing Go.
	PlayerStatePtr->AddToBalance(200);
	
	PlayerStatePtr->SetCurrentSpaceId(0);
	PlayerControllerPtr->MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
}
void ABLPGameState::ChanceCard2(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Advance to Illinois Avenue. If you pass Go, collect $200"));

	const ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(PlayerStatePtr->GetPlayerController());
	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(PlayerStatePtr->GetPawn());

	if (!AvatarPtr) UE_LOG(LogTemp, Warning, TEXT("BLPGameState: AvatarPtr is null"));
	if (!PlayerControllerPtr) UE_LOG(LogTemp, Warning, TEXT("BLPGameState: PlayerControllerPtr is null"));

	// This is required since were not using the RollDice function which normally takes care of passing Go.
	if (PlayerStatePtr->GetCurrentSpaceId() > 24)
	{
		PlayerStatePtr->AddToBalance(200);
	}
		
	PlayerStatePtr->SetCurrentSpaceId(24);
	PlayerControllerPtr->MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
}
void ABLPGameState::ChanceCard3(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Advance to St. Charles Place. If you pass Go, collect $200"));

	const ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(PlayerStatePtr->GetPlayerController());
	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(PlayerStatePtr->GetPawn());

	if (!AvatarPtr) UE_LOG(LogTemp, Warning, TEXT("BLPGameState: AvatarPtr is null"));
	if (!PlayerControllerPtr) UE_LOG(LogTemp, Warning, TEXT("BLPGameState: PlayerControllerPtr is null"));

	// This is required since were not using the RollDice function which normally takes care of passing Go.
	if (PlayerStatePtr->GetCurrentSpaceId() > 11)
	{
		PlayerStatePtr->AddToBalance(200);
	}
	
	PlayerStatePtr->SetCurrentSpaceId(11);
	PlayerControllerPtr->MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
}
void ABLPGameState::ChanceCard4(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Advance to the nearest Railroad. If unowned, you may buy it from the"
								"Bank. If owned, pay the owner twice the rent"));
	
	const ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(PlayerStatePtr->GetPlayerController());
	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(PlayerStatePtr->GetPawn());

	if (!AvatarPtr) UE_LOG(LogTemp, Warning, TEXT("BLPGameState: AvatarPtr is null"));
	if (!PlayerControllerPtr) UE_LOG(LogTemp, Warning, TEXT("BLPGameState: PlayerControllerPtr is null"));

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

	if (!CurrentRailroad) UE_LOG(LogTemp, Warning, TEXT("BLPGameState: Current railroad is null"));
		
	PlayerStatePtr->AddToBalance(-200);
	ABLPPlayerState* OwningPlayer = GetOwnerOfProperty(CurrentRailroad);
	OwningPlayer->AddToBalance(200);
}
void ABLPGameState::ChanceCard5(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Advance to the nearest Railroad. If unowned, you may buy it from the"
								"Bank. If owned, pay the owner twice the rent"));
}
void ABLPGameState::ChanceCard6(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Advance to the nearest Utility. If unowned, you may buy it from the"
								"Bank. If owned, throw the dice and pay the owner ten times the amount"
								"thrown"));
}
void ABLPGameState::ChanceCard7(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Bank pays you a dividend of $50"));
}
void ABLPGameState::ChanceCard8(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Get Out of Jail Free"));
}
void ABLPGameState::ChanceCard9(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Go back 3 Spaces"));
	// const int CurrentSpaceId = PlayerStatePtr->GetCurrentSpaceId();
	// PlayerStatePtr->SetCurrentSpaceId(CurrentSpaceId-3);
}
void ABLPGameState::ChanceCard10(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Go to Jail. Go directly to Jail, do not pass Go, do not collect $200."));

	const ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(PlayerStatePtr->GetPlayerController());
	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(PlayerStatePtr->GetPawn());

	if (!AvatarPtr) UE_LOG(LogTemp, Warning, TEXT("BLPGameState: AvatarPtr is null"));
	if (!PlayerControllerPtr) UE_LOG(LogTemp, Warning, TEXT("BLPGameState: PlayerControllerPtr is null"));

	PlayerControllerPtr->SendToJail(AvatarPtr, PlayerStatePtr, SpaceList);
}
void ABLPGameState::ChanceCard11(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("Make general repairs on all your property. For each house pay $25."
							   "For each hotel pay $100."));
	// TArray<ABLPPropertySpace*> OwnedPropertyList = PlayerStatePtr->GetOwnedPropertyList();
	// int HotelCount = 0;
	// int HouseCount = 0;
	// for (ABLPPropertySpace* Property : OwnedPropertyList)
	// {
	// 	
	// }
}
void ABLPGameState::ChanceCard12(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChanceCard12 has been drawn and initialized!"));
}
void ABLPGameState::ChanceCard13(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChanceCard13 has been drawn and initialized!"));
}
void ABLPGameState::ChanceCard14(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChanceCard14 has been drawn and initialized!"));
}
void ABLPGameState::ChanceCard15(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChanceCard15 has been drawn and initialized!"));
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
