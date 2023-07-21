// Fill out your copyright notice in the Description page of Project Settings.


#include "./BLPUWWinScreen.h"
#include "../Framework/BLPGameInstance.h"
#include "../Framework/Controllers/BLPPlayerController.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UBLPUWWinScreen::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!ExitBtn) return;
	ExitBtn->OnClicked.AddDynamic(this, &UBLPUWWinScreen::ExitBtnClicked);
}

void UBLPUWWinScreen::ExitBtnClicked()
{
	UBLPGameInstance* BLPGameInstancePtr = GetGameInstance<UBLPGameInstance>();
	ABLPPlayerController* BLPPlayerControllerPtr = GetOwningPlayer<ABLPPlayerController>();

	if (!BLPGameInstancePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWWinScreen: BLPGameInstancePtr is null")); return; }
	if (!BLPPlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWWinScreen: BLPPlayerControllerPtr is null")); return; }

	BLPGameInstancePtr->QuitToMainMenu(BLPPlayerControllerPtr);
}
