// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPUWSessionEntry.h"
#include "BLPUWMainMenu.h"

#include "Components/Button.h"

void UBLPUWSessionEntry::Setup(UBLPUWMainMenu* InParent, uint32 InIndex)
{
	// Check SessionEntryBtn for nullptr
	if (!SessionEntryBtn) return;
	// Register callback
	SessionEntryBtn->OnClicked.AddDynamic(this, &UBLPUWSessionEntry::OnClick);
	// Set parent and index of this seession entry
	Parent = InParent;
	Index = InIndex;
}

void UBLPUWSessionEntry::OnClick()
{
	if (Parent)
	{
		// Updated selected index in the MainMenu
		Parent->SetSelectedIndex(Index);
	}
}
