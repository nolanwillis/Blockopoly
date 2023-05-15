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
	UE_LOG(LogTemp, Warning, TEXT("ChanceCard0 has been drawn and initialized!"));
}
void ABLPGameState::ChanceCard1(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChanceCard1 has been drawn and initialized!"));
}
void ABLPGameState::ChanceCard2(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChanceCard2 has been drawn and initialized!"));
}
void ABLPGameState::ChanceCard3(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChanceCard3 has been drawn and initialized!"));
}
void ABLPGameState::ChanceCard4(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChanceCard4 has been drawn and initialized!"));
}
void ABLPGameState::ChanceCard5(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChanceCard5 has been drawn and initialized!"));
}
void ABLPGameState::ChanceCard6(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChanceCard6 has been drawn and initialized!"));
}
void ABLPGameState::ChanceCard7(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChanceCard7 has been drawn and initialized!"));
}
void ABLPGameState::ChanceCard8(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChanceCard8 has been drawn and initialized!"));
}
void ABLPGameState::ChanceCard9(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChanceCard9 has been drawn and initialized!"));
}
void ABLPGameState::ChanceCard10(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChanceCard10 has been drawn and initialized!"));
}
void ABLPGameState::ChanceCard11(ABLPPlayerState* PlayerStatePtr)
{
	UE_LOG(LogTemp, Warning, TEXT("ChanceCard11 has been drawn and initialized!"));
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
