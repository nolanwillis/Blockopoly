// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPUWPauseMenu.h"
#include "../Framework/BLPGameInstance.h"
#include "../Framework/Controllers/BLPPlayerController.h"

#include "Components/Button.h"

void UBLPUWPauseMenu::NativeConstruct()
{
	// Call parent version of function and store result in variable
	Super::NativeConstruct();

	// Bind host/join server functions to OnClicked delegate of host/join button
	if (!MainMenuBtn) return;
	MainMenuBtn->OnClicked.AddDynamic(this, &UBLPUWPauseMenu::MainMenuBtnClicked);
	if (!BackBtn) return;
	BackBtn->OnClicked.AddDynamic(this, &UBLPUWPauseMenu::BackBtnClicked);
}

void UBLPUWPauseMenu::BackBtnClicked()
{
	Remove();
}

void UBLPUWPauseMenu::MainMenuBtnClicked()
{
	UBLPGameInstance* BLPGameInstancePtr = GetGameInstance<UBLPGameInstance>();
	ABLPPlayerController* BLPPlayerControllerPtr = GetOwningPlayer<ABLPPlayerController>();
    
	if (!BLPGameInstancePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerState: BLPGameInstancePtr is null")); return; }
	if (!BLPPlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerState: BLPPlayerControllerPtr is null")); return; }
    	
	BLPGameInstancePtr->QuitToMainMenu(BLPPlayerControllerPtr);
}
