// Fill out your copyright notice in the Description page of Project Settings.
#include "BLPPlayerController.h"
#include "BLPAvatar.h"
#include "BLPGameState.h"
#include "BLPPlayerState.h"
#include "BLPSpace.h"
#include "BLPPropertySpace.h"

#include "GameFramework/Controller.h"

// Server RPC that moves the Avatar to it's DesiredSpaceID
void ABLPPlayerController::Server_Move_Implementation(ABLPAvatar* AvatarPtr, ABLPPlayerState* PlayerStatePtr, const TArray<ABLPSpace*>& LocalSpaceList)
{
	if (!AvatarPtr) { UE_LOG(LogTemp, Warning, TEXT("AvatarPtr is null, from PC")); return; }
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerStatePtr is null, from PC")); return; }
	if (LocalSpaceList.IsEmpty()) { UE_LOG(LogTemp, Warning, TEXT("Sent SpaceList is empty, from PC")); return; }
	
	for (ABLPSpace* Space : LocalSpaceList)
	{
		if (Space->GetSpaceID() == PlayerStatePtr->GetDesiredSpaceID())
		{
			AvatarPtr->SetActorTransform(Space->GetActorTransform() + Space->GetSpawnPointTransform());
		}
	}
}
bool ABLPPlayerController::Server_Move_Validate(ABLPAvatar* AvatarPtr, ABLPPlayerState* PlayerStatePtr, const TArray<ABLPSpace*>& LocalSpaceList){ return true; }

// Server RPC that simulates a dice roll 
void ABLPPlayerController::Server_RollDice_Implementation(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerStatePtr is null, from PC")); return; }
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerStatePtr is null, from PC")); return; }
	if (!PlayerStatePtr->GetIsItMyTurn())
	{
		UE_LOG(LogTemp, Warning, TEXT("It's not your turn!"))
		return;
	}
	
	const int DiceValue = FMath::RandRange(1, 12);
	UE_LOG(LogTemp, Warning, TEXT("You rolled a: %d"), DiceValue);
	int NewDesiredSpaceID = PlayerStatePtr->GetDesiredSpaceID() + DiceValue;
	const int LargestSpaceID = GameStatePtr->GetSpaceList().Num()-1;
	if (NewDesiredSpaceID > LargestSpaceID) NewDesiredSpaceID = DiceValue-1;
	
	PlayerStatePtr->SetDesiredSpaceID(NewDesiredSpaceID);
	
	UE_LOG(LogTemp, Warning, TEXT("DesiredSpaceID is: %d"), PlayerStatePtr->GetDesiredSpaceID());
}
bool ABLPPlayerController::Server_RollDice_Validate(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr){ return true; }

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

// Server RPC that applies the side effect of a newly entered space (ex: if a unowned property take rent)
void ABLPPlayerController::Server_ApplySpaceSideEffect_Implementation(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr)
{
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerStatePtr is null, from PC")); return; }
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("GameStatePtr is null, from PC")); return; }
	
	if (!PlayerStatePtr->GetIsItMyTurn())
	{
		UE_LOG(LogTemp, Warning, TEXT("It's not your turn!"))
		return;
	}
	
	const int LocalDesiredSpaceID = PlayerStatePtr->GetDesiredSpaceID();
	ABLPSpace* EnteredSpace = GameStatePtr->GetSpaceList()[LocalDesiredSpaceID];
	
	if (Cast<ABLPPropertySpace>(EnteredSpace))
	{
		const ABLPPropertySpace* EnteredProperty = Cast<ABLPPropertySpace>(EnteredSpace);
		if (EnteredProperty->GetOwnerID() == -1)
		{
			UE_LOG(LogTemp, Warning, TEXT("This property has no owner"));
			return;
		}
		if (EnteredProperty->GetOwnerID() != PlayerStatePtr->GetPlayerId())
		{
			PlayerStatePtr->AddToBalance(-EnteredProperty->GetRent());
			
			TArray<TObjectPtr<APlayerState>> LocalPlayerArray = GameStatePtr->PlayerArray;
			for (TObjectPtr<APlayerState> const PState : LocalPlayerArray)
			{
				if (PState->GetPlayerId() == EnteredProperty->GetOwnerID())
				{
					if (ABLPPlayerState* OwnerOfProperty = Cast<ABLPPlayerState>(Player))
					{
						OwnerOfProperty->AddToBalance(EnteredProperty->GetRent());
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Owner couldn't be found!"));
					}
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("This is not a property"));
	}
}
bool ABLPPlayerController::Server_ApplySpaceSideEffect_Validate(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr){ return true; }

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
	
	const int LocalDesiredSpaceID = PlayerStatePtr->GetDesiredSpaceID();
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

// Server RPC that allows a user to sell a property (NOT TESTED)
void ABLPPlayerController::Server_SellPropertySpace_Implementation(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr, ABLPPropertySpace* PropertySpacePtr)
{
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerStatePtr is null, from PC")); return; }
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("GameStatePtr is null, from PC")); return; }
	if (!PropertySpacePtr) { UE_LOG(LogTemp, Warning, TEXT("PropertySpacePtr is null, from PC")); return; }
	
	if (!PlayerStatePtr->GetIsItMyTurn())
	{
		UE_LOG(LogTemp, Warning, TEXT("It's not your turn!"))
		return;
	}

	if (PlayerStatePtr->GetPlayerId() != PropertySpacePtr->GetOwnerID())
	{
		UE_LOG(LogTemp, Warning, TEXT("You don't own this property"))
		return;
	}

	PropertySpacePtr->SetOwnerID(-1);
	PlayerStatePtr->RemoveFromOwnedPropertyList(PropertySpacePtr);
	PlayerStatePtr->AddToBalance(PropertySpacePtr->GetSellValue());
}
bool ABLPPlayerController::Server_SellPropertySpace_Validate(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr, ABLPPropertySpace* PropertySpacePtr){ return true; }
