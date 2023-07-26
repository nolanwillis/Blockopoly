// Fill out your copyright notice in the Description page of Project Settings.


#include "./BLPUWPlayerCard.h"
#include "../Framework/State/BLPPlayerState.h"
#include "./BLPUWPropertyMenu.h"

#include "Components/Button.h"

// Should only be used in BLPUWPropertyMenu 
void UBLPUWPlayerCard::Setup(UBLPUWPropertyMenu* InParent, ABLPPlayerState* BLPPropertySpacePtr)
{
	if (!InParent) { UE_LOG(LogTemp, Warning, TEXT("BLPUWPlayerCard: InParent is null")); return; }
	if (!BLPPropertySpacePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWPlayerCard: BLPPropertySpacePtr is null")); return; }
	
	// Bind OnClicked event
	if (!Container) return;
	Container->OnClicked.AddDynamic(this, &UBLPUWPlayerCard::OnClick);
	Parent = InParent;
	AssociatedPlayerState = BLPPropertySpacePtr;
}

void UBLPUWPlayerCard::OnClick()
{
	// Make previous card that was selected dark again
	UBLPUWPlayerCard* LastSelectedCard = Parent->GetSelectedPlayerCard(); 
	if (LastSelectedCard)
	{
		LastSelectedCard->SetRenderOpacity(0.2f);
	}

	// Make this card bright
	SetRenderOpacity(1.0f);
	
	Parent->SetPlayerToTradeWith(AssociatedPlayerState);
	Parent->SetSelectedPlayerCard(this);
	Parent->RefreshPropertyDetails();
	
}
