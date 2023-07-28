// Fill out your copyright notice in the Description page of Project Settings.

#include "BLPPlayerState.h"
#include "../Pawns/BLPAvatar.h"
#include "BLPGameState.h"
#include "../../Items/Spaces/BLPSpace.h"
#include "../../Items/Spaces/BLPPropertySpace.h"
#include "../../Items/Spaces/BLPJailSpace.h"
#include "Blockopoly/Framework/Controllers/BLPPlayerController.h"

#include "Net/UnrealNetwork.h"

ABLPPlayerState::ABLPPlayerState()
{
	SetReplicates(true);
}

void ABLPPlayerState::Client_RefreshUI_Implementation()
{
	RefreshUIDelegate.Broadcast();
}
bool ABLPPlayerState::Client_RefreshUI_Validate(){ return true; }

void ABLPPlayerState::Client_AddNotification_Implementation(const FString& Type, const FString& Heading, const FString& Description)
{
	NotificationDelegate.ExecuteIfBound(Type, Heading, Description);
}
bool ABLPPlayerState::Client_AddNotification_Validate(const FString& Type, const FString& Heading, const FString& Description){ return true; }

void ABLPPlayerState::Client_SimulateMoveLocally_Implementation(const int NewSpaceId)
{
	ABLPAvatar* BLPAvatarPtr = Cast<ABLPAvatar>(GetPawn());
	const ABLPGameState* BLPGameStatePtr = Cast<ABLPGameState>(GetWorld()->GetGameState());
	
	if (!BLPAvatarPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerState: BLPAvatarPtr is null")); return; }
	if (!BLPGameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerState: BLPGameStatePtr is null")); return; }

	TArray<ABLPSpace*> SpaceList = BLPGameStatePtr->GetSpaceList();
	
	for (ABLPSpace* Space : SpaceList)
	{
		if (Space->GetSpaceID() == NewSpaceId)
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
			BLPAvatarPtr->SetActorLocationAndRotation(NewLocation, NewRotation);
		}
	}
}
bool ABLPPlayerState::Client_SimulateMoveLocally_Validate(const int NewSpaceId){ return true; }

void ABLPPlayerState::Client_DisplayWinScreen_Implementation(const FString& WinnersName)
{
	ABLPPlayerController* BLPPlayerControllerPtr = Cast<ABLPPlayerController>(GetPlayerController()); 
	if (!BLPPlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerState: BLPPlayerControllerPtr is null")); return; }
	BLPPlayerControllerPtr->LoadWinScreen(WinnersName);
}
bool ABLPPlayerState::Client_DisplayWinScreen_Validate(const FString& WinnersName){ return true; }

void ABLPPlayerState::Client_AddSaleRequest_Implementation(const FPropertySaleData& SaleData)
{
	SaleRequestDelegate.ExecuteIfBound(SaleData);
}
bool ABLPPlayerState::Client_AddSaleRequest_Validate(const FPropertySaleData& SaleData){ return true; }
void ABLPPlayerState::Client_AddSaleResponse_Implementation(const FPropertySaleData& SaleData, const bool Status)
{
	SaleResponseDelegate.ExecuteIfBound(SaleData, Status);
}
bool ABLPPlayerState::Client_AddSaleResponse_Validate(const FPropertySaleData& SaleData, const bool Status){ return true; }

void ABLPPlayerState::OnRep_PlayerUpId() const
{
	PlayerUpIdDelegate.Broadcast();
}

// Notifies UI of credit change, so UI reflects correct credit amount
void ABLPPlayerState::OnRep_CreditBalance() const
{
	UE_LOG(LogTemp, Warning, TEXT("New Balance: %d"), CreditBalance);
	OnBalanceChangedDelegate.Broadcast(CreditBalance);
}

void ABLPPlayerState::OnRep_OwnedPropertyList() const
{
	UE_LOG(LogTemp, Warning, TEXT("Owned Property List Updated!"));
	UE_LOG(LogTemp, Warning, TEXT("Owned Property List: "));
	UE_LOG(LogTemp, Warning, TEXT("///////////////////////"));
	for (ABLPPropertySpace* Property : OwnedPropertyList)
	{
		UE_LOG(LogTemp, Warning, TEXT("|Name: %s | Rent: %d|"), *Property->GetName(), Property->GetCurrentRent());
	}
	UE_LOG(LogTemp, Warning, TEXT("///////////////////////"));

	OnPropertyListChangedDelegate.ExecuteIfBound();
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
	JailSkipDelegate.ExecuteIfBound(JailSkipCounter);
}

void ABLPPlayerState::OnRep_PlayerCount()
{
	RefreshUIDelegate.Broadcast();
}

void ABLPPlayerState::OnRep_CanBuyCurrentProperty()
{
	CanBuyDelegate.ExecuteIfBound(CanBuyCurrentProperty);
}

void ABLPPlayerState::OnRep_HasRolled()
{
	if (HasRolled)
	{
		UE_LOG(LogTemp, Warning, TEXT("HasRolled is true"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HasRolled is false"));
	}
	
	HasRolledDelegate.ExecuteIfBound(HasRolled);
}

void ABLPPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Here we list the variables we want to replicate
	DOREPLIFETIME(ABLPPlayerState, PlayerUpId);
	DOREPLIFETIME(ABLPPlayerState, CreditBalance);
	DOREPLIFETIME(ABLPPlayerState, CurrentSpaceId);
	DOREPLIFETIME(ABLPPlayerState, OwnedPropertyList);
	DOREPLIFETIME(ABLPPlayerState, JailCounter);
	DOREPLIFETIME(ABLPPlayerState, JailSkipCounter);
	DOREPLIFETIME(ABLPPlayerState, CanBuyCurrentProperty);
	DOREPLIFETIME(ABLPPlayerState, HasRolled);
	DOREPLIFETIME(ABLPPlayerState, PlayerCount);
	DOREPLIFETIME(ABLPPlayerState, BLPPlayerId);
	DOREPLIFETIME(ABLPPlayerState, IsLeaving);
}

