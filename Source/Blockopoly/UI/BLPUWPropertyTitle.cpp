// Fill out your copyright notice in the Description page of Project Settings.


#include "./BLPUWPropertyTitle.h"

#include "BLPUWPropertyMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UBLPUWPropertyTitle::Setup(UBLPUWPropertyMenu* InParent, ABLPPropertySpace* PropertySpacePtr)
{
	// Bind OnClicked event
	if (!Button) return;
	Button->OnClicked.AddDynamic(this, &UBLPUWPropertyTitle::OnClick);
	Parent = InParent;
	AssociatedProperty = PropertySpacePtr;
}

void UBLPUWPropertyTitle::OnClick()
{
	Parent->SetSelectedPropertySpace(AssociatedProperty);
	Parent->SetSelectedPropertyTitle(this);
	Parent->RefreshPropertyDetails();
	
	if (AssociatedProperty->GetHasPendingSale())
	{
		Parent->GetSellBtn()->SetVisibility(ESlateVisibility::Collapsed);
		Parent->GetPendingSaleText()->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Parent->GetSellBtn()->SetVisibility(ESlateVisibility::Visible);
		Parent->GetPendingSaleText()->SetVisibility(ESlateVisibility::Hidden);
	}
}

