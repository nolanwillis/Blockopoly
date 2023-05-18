// Fill out your copyright notice in the Description page of Project Settings.

#include "BLPPlayerState.h"
#include "../Pawns/BLPAvatar.h"
#include "BLPGameState.h"
#include "../../Items/Spaces/BLPSpace.h"
#include "../../Items/Spaces/BLPPropertySpace.h"

#include "Net/UnrealNetwork.h"

ABLPPlayerState::ABLPPlayerState()
{
	SetReplicates(true);
}

// Notifies UI of credit change, so UI reflects correct credit amount
void ABLPPlayerState::OnRep_CreditBalance() const
{
	UE_LOG(LogTemp, Warning, TEXT("New Balance: %d"), CreditBalance);
}

// Simulates Avatar movement locally when CurrentSpaceId is changed
void ABLPPlayerState::OnRep_DesiredSpaceID() const
{
	UE_LOG(LogTemp, Warning, TEXT("CurrentSpaceId Updated"));

	ABLPAvatar* AvatarPtr = Cast<ABLPAvatar>(GetPawn());
	ABLPGameState* GameStatePtr = Cast<ABLPGameState>(GetWorld()->GetGameState());
	
	if (!AvatarPtr) { UE_LOG(LogTemp, Warning, TEXT("AvatarPtr is null, from PS")); return; }
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("AvatarPtr is null, from PS")); return; }

	TArray<ABLPSpace*> LocalSpaceList = GameStatePtr->GetSpaceList();
	
	for (ABLPSpace* Space : LocalSpaceList)
	{
		if (Space->GetSpaceID() == CurrentSpaceId)
		{
			AvatarPtr->SetActorTransform(Space->GetActorTransform() + Space->GetSpawnPointTransform());
		}
	}
}

// Notifies UI if its this players turn, so the turn UI buttons appear (roll, finish turn, etc.)
void ABLPPlayerState::OnRep_IsItMyTurn() const
{
	if (IsItMyTurn)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: It's my turn!"), *GetPlayerName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s I finished my turn"), *GetPlayerName());
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

void ABLPPlayerState::OnRep_JailTurnCounter()
{
	if (InJailTurnCounter == 3)
	{
		InJail = true;
	}
	if (InJailTurnCounter == 0)
	{
		InJail = false;
	}
}

void ABLPPlayerState::OnRep_GetOutOfJailCounter()
{
	// TODO: Update UI by removing or adding get out of jail card and in jail warning
}

void ABLPPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Here we list the variables we want to replicate
	DOREPLIFETIME(ABLPPlayerState, CreditBalance);
	DOREPLIFETIME(ABLPPlayerState, CurrentSpaceId);
	DOREPLIFETIME(ABLPPlayerState, IsItMyTurn);
	DOREPLIFETIME(ABLPPlayerState, OwnedPropertyList);
	DOREPLIFETIME(ABLPPlayerState, InJailTurnCounter);
	DOREPLIFETIME(ABLPPlayerState, GetOutOfJailCounter);
}

