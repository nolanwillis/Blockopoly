// Fill out your copyright notice in the Description page of Project Settings.

#include "BLPPlayerState.h"
#include "../Pawns/BLPAvatar.h"
#include "BLPGameState.h"
#include "../../Items/Spaces/BLPSpace.h"
#include "../../Items/Spaces/BLPPropertySpace.h"
#include "../../Items/Spaces/BLPJailSpace.h"

#include "Net/UnrealNetwork.h"

ABLPPlayerState::ABLPPlayerState()
{
	SetReplicates(true);
}

// Notifies UI of credit change, so UI reflects correct credit amount
void ABLPPlayerState::OnRep_CreditBalance() const
{
	UE_LOG(LogTemp, Warning, TEXT("New Balance: %d"), CreditBalance);
	if (!OnBalanceChangedDelegate.IsBound()) return;
	OnBalanceChangedDelegate.Broadcast(CreditBalance);
}

// Simulates Avatar movement locally when CurrentSpaceId is changed
void ABLPPlayerState::OnRep_DesiredSpaceID()
{
	UE_LOG(LogTemp, Warning, TEXT("CurrentSpaceId Updated"));

	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(GetPawn());
	ABLPGameState* GameStatePtr = Cast<ABLPGameState>(GetWorld()->GetGameState());
	
	if (!AvatarPtr) { UE_LOG(LogTemp, Warning, TEXT("AvatarPtr is null, from PS")); return; }
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("AvatarPtr is null, from PS")); return; }

	TArray<ABLPSpace*> SpaceList = GameStatePtr->GetSpaceList();
	
	for (ABLPSpace* Space : SpaceList)
	{
		if (Space->GetSpaceID() == CurrentSpaceId)
		{
			FSpawnPoint* NewSpawnPoint = nullptr;

			if (JailCounter == 3)
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
			CurrentSpawnPoint = NewSpawnPoint;
			
			const FVector NewLocation = Space->GetActorTransform().GetLocation() + NewSpawnPoint->Transform.GetLocation();
			const FRotator NewRotation = Space->GetActorTransform().GetRotation().Rotator();
			AvatarPtr->SetActorLocationAndRotation(NewLocation, NewRotation);
		}
	}
}

// Notifies UI if its this players turn, so the turn UI buttons appear (roll, finish turn, etc.)
void ABLPPlayerState::OnRep_IsItMyTurn() const 
{
	if (IsItMyTurn)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: It's my turn!"), *GetPlayerName());
		if (!ItsMyTurnDelegate.IsBound()) return;
		ItsMyTurnDelegate.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s I finished my turn"), *GetPlayerName());
		if (!ItsNotMyTurnDelegate.IsBound()) return;
		ItsNotMyTurnDelegate.Broadcast();
	}
}

void ABLPPlayerState::OnRep_OwnedPropertyList() const
{
	UE_LOG(LogTemp, Warning, TEXT("Owned Property List Updated!"));
	UE_LOG(LogTemp, Warning, TEXT("Owned Property List: "));
	UE_LOG(LogTemp, Warning, TEXT("///////////////////////"));
	for (ABLPPropertySpace* Property : OwnedPropertyList)
	{
		UE_LOG(LogTemp, Warning, TEXT("|Name: %s | Rent: %d|"), *Property->GetName(), Property->GetRent());
	}
	UE_LOG(LogTemp, Warning, TEXT("///////////////////////"));
}

void ABLPPlayerState::OnRep_JailCounter()
{
	if (JailCounter > 0)
	{
		InJailDelegate.ExecuteIfBound(JailCounter);
	}
	else if (JailCounter == 0)
	{
		OutOfJailDelegate.ExecuteIfBound();
	}
}

void ABLPPlayerState::OnRep_JailSkipCounter()
{
	UE_LOG(LogTemp, Warning, TEXT("Get Out of Jail Free Card added"));
}

void ABLPPlayerState::OnRep_PlayerCount()
{
	PlayerCountDelegate.ExecuteIfBound();
}

void ABLPPlayerState::OnRep_CanBuyCurrentProperty()
{
	UE_LOG(LogTemp, Warning, TEXT("Can Buy Current Property Updated"));
	CanBuyDelegate.ExecuteIfBound(CanBuyCurrentProperty);
}

void ABLPPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Here we list the variables we want to replicate
	DOREPLIFETIME(ABLPPlayerState, CreditBalance);
	DOREPLIFETIME(ABLPPlayerState, CurrentSpaceId);
	DOREPLIFETIME(ABLPPlayerState, IsItMyTurn);
	DOREPLIFETIME(ABLPPlayerState, OwnedPropertyList);
	DOREPLIFETIME(ABLPPlayerState, JailCounter);
	DOREPLIFETIME(ABLPPlayerState, JailSkipCounter);
	DOREPLIFETIME(ABLPPlayerState, CanBuyCurrentProperty);
}

