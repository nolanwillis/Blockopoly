// Fill out your copyright notice in the Description page of Project Settings.

#include "BLPGameState.h"
#include "BLPPlayerState.h"

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