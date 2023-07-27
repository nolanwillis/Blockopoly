// Fill out your copyright notice in the Description page of Project Settings.


#include "./BLPUWPropertyTitle.h"

#include "BLPUWPropertyMenu.h"
#include "Blockopoly/Framework/State/BLPPlayerState.h"
#include "Blockopoly/Items/Spaces/BLPEstatePropertySpace.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UBLPUWPropertyTitle::Setup(UBLPUWPropertyMenu* InParent, ABLPPropertySpace* PropertySpacePtr)
{
	// Bind OnClicked event
	if (!PropertyTitleContainer) return;
	PropertyTitleContainer->OnClicked.AddDynamic(this, &UBLPUWPropertyTitle::OnClick);
	Parent = InParent;
	AssociatedProperty = PropertySpacePtr;
}

void UBLPUWPropertyTitle::OnClick()
{
	ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
	if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWPropertyTitle: BLPPlayerStatePtr is null")); return; }

	Parent->SetSelectedPropertySpace(AssociatedProperty);
	Parent->SetSelectedPropertyTitle(this);
	Parent->RefreshPropertyDetails();
	Parent->RefreshPropertyManagementButtons();
}

