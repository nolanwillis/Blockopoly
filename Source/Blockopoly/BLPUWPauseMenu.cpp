// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPUWPauseMenu.h"
#include "BLPGameInstance.h"

#include "Components/Button.h"

bool UBLPUWPauseMenu::Initialize()
{
	// Call parent version of function and store result in variable
	bool Success = Super::Initialize();
	if (!Success) return false;

	// Bind host/join server functions to OnClicked delegate of host/join button
	if (!MainMenuBtn) return false;
	MainMenuBtn->OnClicked.AddDynamic(this, &UBLPUWPauseMenu::MainMenuBtnClicked);
	if (!BackBtn) return false;
	BackBtn->OnClicked.AddDynamic(this, &UBLPUWPauseMenu::BackBtnClicked);
	return true;
}

void UBLPUWPauseMenu::BackBtnClicked()
{
	Remove();
}

void UBLPUWPauseMenu::MainMenuBtnClicked()
{
	UBLPGameInstance* GameInstance = Cast<UBLPGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->QuitToMainMenu();
	}
}
