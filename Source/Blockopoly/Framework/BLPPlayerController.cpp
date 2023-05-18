// Fill out your copyright notice in the Description page of Project Settings.
#include "BLPPlayerController.h"
#include "./Pawns/BLPAvatar.h"
#include "./State/BLPGameState.h"
#include "./State/BLPPlayerState.h"
#include "../Items/Spaces/BLPSpace.h"
#include "../Items/Spaces/BLPPropertySpace.h"
#include "../Items/Spaces/BLPEstatePropertySpace.h"
#include "../Items/Spaces/BLPChanceSpace.h"
#include "../Items/Spaces/BLPChestSpace.h"

#include "GameFramework/Controller.h"
#include "Components/StaticMeshComponent.h"

// Server RPC that moves the Avatar to it's CurrentSpaceId
void ABLPPlayerController::Server_TakeTurn_Implementation(ABLPAvatar* AvatarPtr, ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	if (!AvatarPtr) { UE_LOG(LogTemp, Warning, TEXT("AvatarPtr is null, from PC")); return; }
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerStatePtr is null, from PC")); return; }
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("GameStatePtr is null, from PC")); return; }

	if (!PlayerStatePtr->GetIsItMyTurn())
	{
		UE_LOG(LogTemp, Warning, TEXT("It's not your turn!"))
		return;
	}

	const TArray<ABLPSpace*> SpaceList = GameStatePtr->GetSpaceList();

	if (SpaceList.IsEmpty()) { UE_LOG(LogTemp, Warning, TEXT("Sent SpaceList is empty, from PC")); return; }

	RollDice(PlayerStatePtr, GameStatePtr);
	MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
	ApplySpaceSideEffect(PlayerStatePtr, GameStatePtr);
}
bool ABLPPlayerController::Server_TakeTurn_Validate(ABLPAvatar* AvatarPtr, ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr){ return true; }

// Server RPC that finishes the current turn and moves to the next
void ABLPPlayerController::Server_FinishTurn_Implementation(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerStatePtr is null, from PC")); return; }
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("GameStatePtr is null, from PC")); return; }

	if (!PlayerStatePtr->GetIsItMyTurn())
	{
		UE_LOG(LogTemp, Warning, TEXT("It's not your turn!"))
		return;
	}
	
	PlayerStatePtr->SetIsItMyTurn(false);
	
	const TArray<TObjectPtr<APlayerState>> LocalPlayerArray = GameStatePtr->PlayerArray;
	const int PlayerCount = LocalPlayerArray.Num();
	
	if (GameStatePtr->GetPlayerUpIndex() < PlayerCount-1)
	{
		GameStatePtr->SetPlayerUpIndex(GameStatePtr->GetPlayerUpIndex()+1);
	}
	else
	{
		GameStatePtr->SetPlayerUpIndex(0);
	}

	// Update the PlayerState of the player with the next turn
	Cast<ABLPPlayerState>(LocalPlayerArray[GameStatePtr->GetPlayerUpIndex()])->SetIsItMyTurn(true);
}
bool ABLPPlayerController::Server_FinishTurn_Validate(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr){ return true; }

// Server RPC that allows a user to buy a property that has no owner
void ABLPPlayerController::Server_BuyPropertySpace_Implementation(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerStatePtr is null, from PC")); return; }
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("GameStatePtr is null, from PC")); return; }
	
	if (!PlayerStatePtr->GetIsItMyTurn())
	{
		UE_LOG(LogTemp, Warning, TEXT("It's not your turn!"))
		return;
	}
	
	const int LocalDesiredSpaceID = PlayerStatePtr->GetCurrentSpaceId();
	const TArray<ABLPSpace*> LocalSpaceList = GameStatePtr->GetSpaceList();
	ABLPPropertySpace* PropertySpaceToPurchase = nullptr;
	for (ABLPSpace* Space : LocalSpaceList)
	{
		if (Space->GetSpaceID() == LocalDesiredSpaceID) PropertySpaceToPurchase = Cast<ABLPPropertySpace>(Space);
	}

	if (!PropertySpaceToPurchase)
	{
		UE_LOG(LogTemp, Warning, TEXT("ERROR: The property your trying to buy could not be found"))
		return;
	}

	if (PropertySpaceToPurchase->GetOwnerID() == PlayerStatePtr->GetPlayerId())
	{
		UE_LOG(LogTemp, Warning, TEXT("You already own this property!"));
		return;
	}

	if (!PropertySpaceToPurchase)
	{
		UE_LOG(LogTemp, Warning, TEXT("The Space your attempting to purchase is not a PropertySpace"));
		return;
	}

	if (PropertySpaceToPurchase->GetOwnerID() != -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("This property is already owned!"));
		return;
	}

	const int RemainingBalance = PlayerStatePtr->GetBalance() - PropertySpaceToPurchase->GetPurchaseCost();
	if (RemainingBalance < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("You need %d more credits"), abs(RemainingBalance));
		return;
	}
	
	PropertySpaceToPurchase->SetOwnerID(PlayerStatePtr->GetPlayerId());
	PlayerStatePtr->AddToOwnedPropertyList(PropertySpaceToPurchase);
	GameStatePtr->RemoveFromAvailablePropertySpaceList(PropertySpaceToPurchase);
	PlayerStatePtr->AddToBalance(-PropertySpaceToPurchase->GetPurchaseCost());
}
bool ABLPPlayerController::Server_BuyPropertySpace_Validate(ABLPPlayerState* PlayerStatePtr,ABLPGameState* GameStatePtr){ return true; }

// Server RPC that allows a user to sell a property
void ABLPPlayerController::Server_SellPropertySpace_Implementation(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr, const int& SpaceID)
{
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: GameStatePtr is null")); return; }
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: PlayerStatePtr is null")); return; }
	if (SpaceID < 0) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: SpaceID invalid")); return; }
	
	if (!PlayerStatePtr->GetIsItMyTurn())
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: It's not your turn!"))
		return;
	}
	
	ABLPPropertySpace* PropertySpacePtr = nullptr;

	TArray<ABLPSpace*> LocalSpaceList = GameStatePtr->GetSpaceList();
	for (ABLPSpace* Space : LocalSpaceList)
	{
		if (Space->GetSpaceID() == SpaceID) PropertySpacePtr = Cast<ABLPPropertySpace>(Space);
	}

	if (!PropertySpacePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: This is not a property!")); return; }
	
	if (PlayerStatePtr->GetPlayerId() != PropertySpacePtr->GetOwnerID())
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: You don't own this property!"))
		return;
	}

	PropertySpacePtr->SetOwnerID(-1);
	PlayerStatePtr->RemoveFromOwnedPropertyList(PropertySpacePtr);
	PlayerStatePtr->AddToBalance(PropertySpacePtr->GetSellValue());
	GameStatePtr->AddToAvailablePropertySpaceList(PropertySpacePtr);
}
bool ABLPPlayerController::Server_SellPropertySpace_Validate(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr, const int& SpaceID){ return true; }

// Server RPC that enables a user to build a building on a property
void ABLPPlayerController::Server_BuyBuilding_Implementation(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr, const int& SpaceID)
{
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: GameStatePtr is null")); return; }
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: PlayerStatePtr is null")); return; }
	if (SpaceID < 0) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: SpaceID invalid")); return; }
	
	if (!PlayerStatePtr->GetIsItMyTurn())
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: It's not your turn!"))
		return;
	}
	
	ABLPEstatePropertySpace* EstatePropertySpacePtr = nullptr;

	TArray<ABLPSpace*> LocalSpaceList = GameStatePtr->GetSpaceList();
	for (ABLPSpace* Space : LocalSpaceList)
	{
		if (Space->GetSpaceID() == SpaceID) EstatePropertySpacePtr = Cast<ABLPEstatePropertySpace>(Space);
	}

	if (!EstatePropertySpacePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: This is not an estate property!")); return; }
	
	if (PlayerStatePtr->GetPlayerId() != EstatePropertySpacePtr->GetOwnerID())
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: You don't own this estate property!"))
		return;
	}

	PlayerStatePtr->AddToBalance(-EstatePropertySpacePtr->GetBuildingCost());
	const int UpdatedBuildingCount = EstatePropertySpacePtr->GetBuildingCount() + 1;
	EstatePropertySpacePtr->SetBuildingCount(UpdatedBuildingCount);
	UpdateBuildings(EstatePropertySpacePtr, UpdatedBuildingCount);
}
bool ABLPPlayerController::Server_BuyBuilding_Validate(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr,const int& SpaceID){ return true; }

// Simulates a dice roll
void ABLPPlayerController::RollDice(ABLPPlayerState* PlayerStatePtr, const ABLPGameState* GameStatePtr) const
{
	const int DiceValue = FMath::RandRange(1, 12);
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
	
	UE_LOG(LogTemp, Warning, TEXT("CurrentSpaceId is: %d"), PlayerStatePtr->GetCurrentSpaceId());
}

// Moves player (should always be called from the server)
void ABLPPlayerController::MovePlayer(ABLPAvatar* AvatarPtr, const ABLPPlayerState* PlayerStatePtr, const TArray<ABLPSpace*>& SpaceList) const
{
	for (ABLPSpace* Space : SpaceList)
	{
		if (Space->GetSpaceID() == PlayerStatePtr->GetCurrentSpaceId())
		{
			AvatarPtr->SetActorTransform(Space->GetActorTransform() + Space->GetSpawnPointTransform());
		}
	}
}

void ABLPPlayerController::SendToJail(ABLPAvatar* AvatarPtr, ABLPPlayerState* PlayerStatePtr, const TArray<ABLPSpace*>& SpaceList) const
{
	if (!AvatarPtr) UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: AvatarPtr is null"));
	if (!PlayerStatePtr) UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: PlayerStatePtr is null"));
	PlayerStatePtr->SetInJailTurnCounter(3);
	PlayerStatePtr->SetCurrentSpaceId(10);
	MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
}

// Applies correct side effect depending on what space is landed on
void ABLPPlayerController::ApplySpaceSideEffect(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	const int EnteredSpaceID = PlayerStatePtr->GetCurrentSpaceId();
	ABLPSpace* EnteredSpace = GameStatePtr->GetSpaceList()[EnteredSpaceID];
	if (!EnteredSpace)
	{
		UE_LOG(LogTemp, Warning, TEXT("Entered space could not be found"));
		return;
	}
	
	if (Cast<ABLPPropertySpace>(EnteredSpace))
	{
		const ABLPPropertySpace* EnteredPropertySpace = Cast<ABLPPropertySpace>(EnteredSpace);
		PropertySpaceSideEffect(PlayerStatePtr, GameStatePtr, EnteredPropertySpace);
	}
	else if (Cast<ABLPChanceSpace>(EnteredSpace))
	{
		DrawChanceCard(GameStatePtr);
	}
	else if (Cast<ABLPChestSpace>(EnteredSpace))
	{
		DrawChestCard(GameStatePtr);
	}
}

// Collects rent from player if they do not own the property they move to
void ABLPPlayerController::PropertySpaceSideEffect(ABLPPlayerState* PlayerStatePtr, const ABLPGameState* GameStatePtr, const ABLPPropertySpace* EnteredPropertySpace) const
{
	if (EnteredPropertySpace->GetOwnerID() == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("This property has no owner, from PC"));
		return;
	}
	if (EnteredPropertySpace->GetOwnerID() != PlayerStatePtr->GetPlayerId())
	{
		PlayerStatePtr->AddToBalance(-EnteredPropertySpace->GetRent());

		ABLPPlayerState* OwnerOfProperty = GameStatePtr->GetOwnerOfProperty(EnteredPropertySpace);
			
		if (!OwnerOfProperty) UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: Owner of property could not be found!"));

		OwnerOfProperty->AddToBalance(EnteredPropertySpace->GetRent());
	}
}

// Updates the amount of buildings on an estate property
void ABLPPlayerController::UpdateBuildings(const ABLPEstatePropertySpace* EstatePropertySpacePtr, const int& BuildingCount)
{
	if (BuildingCount == 1)
	{
		UStaticMeshComponent* HousePtr = EstatePropertySpacePtr->GetHouse0();
		HousePtr->SetVisibility(true, false);
		UE_LOG(LogTemp, Warning, TEXT("BLPEstatePropertySpace: You've built your 1st house"));
	}
	else if (BuildingCount == 2)
	{
		UStaticMeshComponent* HousePtr = EstatePropertySpacePtr->GetHouse1();
		HousePtr->SetVisibility(true, false);
		UE_LOG(LogTemp, Warning, TEXT("BLPEstatePropertySpace: You've built a 2nd house"));
	}
	else if (BuildingCount == 3)
	{
		UStaticMeshComponent* HousePtr = EstatePropertySpacePtr->GetHouse2();
		HousePtr->SetVisibility(true, false);
		UE_LOG(LogTemp, Warning, TEXT("BLPEstatePropertySpace: You've built a 3rd house"));
		
	}
	else if (BuildingCount == 4)
	{
		UStaticMeshComponent* HousePtr = EstatePropertySpacePtr->GetHouse3();
		HousePtr->SetVisibility(true, false);
		UE_LOG(LogTemp, Warning, TEXT("BLPEstatePropertySpace: You've built a 4th house"));
	}
	else if (BuildingCount == 5)
	{
		UStaticMeshComponent* HotelPtr = EstatePropertySpacePtr->GetHotel();
		HotelPtr->SetVisibility(true, false);
		UE_LOG(LogTemp, Warning, TEXT("BLPEstatePropertySpace: You've built a hotel"));
	}
	
}

void ABLPPlayerController::DrawChanceCard(ABLPGameState* GameStatePtr)
{
}

void ABLPPlayerController::DrawChestCard(ABLPGameState* GameStatePtr)
{
}

