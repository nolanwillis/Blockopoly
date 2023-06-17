// Fill out your copyright notice in the Description page of Project Settings.
#include "BLPPlayerController.h"
#include "./Pawns/BLPAvatar.h"
#include "./State/BLPGameState.h"
#include "./State/BLPPlayerState.h"
#include "../Items/Spaces/BLPSpace.h"
#include "../Items/Spaces/BLPChanceSpace.h"
#include "../Items/Spaces/BLPChestSpace.h"
#include "../Items/Spaces/BLPPropertySpace.h"
#include "../Items/Spaces/BLPEstatePropertySpace.h"
#include "../Items/Spaces/BLPGoToJailSpace.h"
#include "../Items/Spaces/BLPJailSpace.h"
#include "../UI/BLPUWGameMenu.h"

#include "GameFramework/Controller.h"
#include "Components/StaticMeshComponent.h"

ABLPPlayerController::ABLPPlayerController()
{
	// Gets reference to WBP_GameMenu
	const ConstructorHelpers::FClassFinder<UUserWidget> WBP_GameMenu(TEXT("/Game/Core/UI/WBP_GameMenu"));
	if (!WBP_GameMenu.Class) return;
	GameMenuClass = WBP_GameMenu.Class;
}

void ABLPPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	if (IsLocalPlayerController())
	{
		// Construct the a WBP_GameMenu
		if (!GameMenuClass) return;
		GameMenu = CreateWidget<UBLPUWGameMenu>(this, GameMenuClass);
		if (!GameMenu) return;
		GameMenu->Setup();
	}
}

// Server RPC that moves the Avatar to it's CurrentSpaceId
void ABLPPlayerController::Server_Roll_Implementation(ABLPAvatar* AvatarPtr, ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	if (!AvatarPtr) { UE_LOG(LogTemp, Warning, TEXT("AvatarPtr is null, from PC")); return; }
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerStatePtr is null, from PC")); return; }
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("GameStatePtr is null, from PC")); return; }

	if (!PlayerStatePtr->GetIsItMyTurn())
	{
		UE_LOG(LogTemp, Warning, TEXT("It's not your turn!"))
		return;
	}

	if (PlayerStatePtr->GetJailCounter() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Your in jail for %d more turns!"), PlayerStatePtr->GetJailCounter());
		return;
	}
	
	const TArray<ABLPSpace*> SpaceList = GameStatePtr->GetSpaceList();

	if (SpaceList.IsEmpty()) { UE_LOG(LogTemp, Warning, TEXT("Sent SpaceList is empty, from PC")); return; }
	
	RollDice(PlayerStatePtr, GameStatePtr);
	MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
	ApplySpaceSideEffect(PlayerStatePtr, GameStatePtr);
	CheckIfPropertyIsForSale(PlayerStatePtr, GameStatePtr);

	PlayerStatePtr->SetHasRolled(true);
}
bool ABLPPlayerController::Server_Roll_Validate(ABLPAvatar* AvatarPtr, ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr){ return true; }

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
	
	PropertySpaceToPurchase->SetOwnerID(PlayerStatePtr->GetPlayerId());
	PlayerStatePtr->AddToOwnedPropertyList(PropertySpaceToPurchase);
	GameStatePtr->RemoveFromAvailablePropertySpaceList(PropertySpaceToPurchase);
	PlayerStatePtr->AddToBalance(-PropertySpaceToPurchase->GetPurchaseCost());
	PlayerStatePtr->SetCanBuyCurrentProperty(false);
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
	PlayerStatePtr->AddToBalance(PropertySpacePtr->GetMortgageValue());
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
	
	ABLPEstatePropertySpace*  EstatePropertySpacePtr = Cast<ABLPEstatePropertySpace>(GameStatePtr->GetSpaceFromId(SpaceID));

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
void ABLPPlayerController::RollDice(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr) const
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

	GameStatePtr->AddRollNotificationToUI(PlayerStatePtr->GetPlayerName(), DiceValue);
}

// Moves player (should always be called from the server)
void ABLPPlayerController::MovePlayer(ABLPAvatar* AvatarPtr, ABLPPlayerState* PlayerStatePtr, const TArray<ABLPSpace*>& SpaceList) const
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
		}
	}
}

void ABLPPlayerController::SendToJail(ABLPPlayerState* PlayerStatePtr, const TArray<ABLPSpace*>& SpaceList) const
{
	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(PlayerStatePtr->GetPawn());
	if (!AvatarPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: AvatarPtr is null")); return; }
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerController: PlayerStatePtr is null")); return; }
	
	PlayerStatePtr->SetJailCounter(3);
	PlayerStatePtr->SetCurrentSpaceId(10);
	MovePlayer(AvatarPtr, PlayerStatePtr, SpaceList);
}

// Updates the amount of buildings on an estate property
void ABLPPlayerController::UpdateBuildings(const ABLPEstatePropertySpace* EstatePropertySpacePtr, const int& BuildingCount)

{
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

// Applies correct side effect depending on what space is landed on
void ABLPPlayerController::ApplySpaceSideEffect(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
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
}

// Collects rent from player if they do not own the property they move to
void ABLPPlayerController::ChargeRent(ABLPPlayerState* PlayerStatePtr, const ABLPGameState* GameStatePtr, const ABLPPropertySpace* EnteredPropertySpace) const
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

void ABLPPlayerController::DrawChanceCard(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	GameStatePtr->DrawChanceCard(PlayerStatePtr);
}

void ABLPPlayerController::DrawChestCard(ABLPPlayerState* PlayerStatePtr,ABLPGameState* GameStatePtr)
{
	GameStatePtr->DrawChestCard(PlayerStatePtr);
}

void ABLPPlayerController::CheckIfPropertyIsForSale(ABLPPlayerState* PlayerStatePtr, const ABLPGameState* GameStatePtr) const
{
	ABLPSpace* Space = GameStatePtr->GetSpaceFromId(PlayerStatePtr->GetCurrentSpaceId());
	if (const ABLPPropertySpace* PropertySpace = Cast<ABLPPropertySpace>(Space))
	{
		if (PropertySpace->GetOwnerID() == -1)
		{
			PlayerStatePtr->SetCanBuyCurrentProperty(true);
		}
		else
		{
			PlayerStatePtr->SetCanBuyCurrentProperty(false);
		}
	}
}

