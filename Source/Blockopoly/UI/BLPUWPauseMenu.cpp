// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPUWPauseMenu.h"
#include "BLPUWSettingsMenu.h"
#include "../Framework/BLPGameInstance.h"
#include "../Framework/Controllers/BLPPlayerController.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

UBLPUWPauseMenu::UBLPUWPauseMenu()
{
	// Gets reference to WBP_SettingsMenu
	ConstructorHelpers::FClassFinder<UUserWidget> WBP_SettingsMenu(TEXT("/Game/Core/UI/WBP_SettingsMenu"));
	if (!WBP_SettingsMenu.Class) return;
	SettingsMenuClass = WBP_SettingsMenu.Class;
}

void UBLPUWPauseMenu::NativeConstruct()
{
	// Call parent version of function and store result in variable
	Super::NativeConstruct();

	// Bind host/join server functions to OnClicked delegate of host/join button
	if (!MainMenuBtn) return;
	MainMenuBtn->OnClicked.AddDynamic(this, &UBLPUWPauseMenu::MainMenuBtnClicked);
	if (!SettingsMenuBtn) return;
	SettingsMenuBtn->OnClicked.AddDynamic(this, &UBLPUWPauseMenu::SettingsMenuBtnClicked);
	if (!BackBtn) return;
	BackBtn->OnClicked.AddDynamic(this, &UBLPUWPauseMenu::BackBtnClicked);

	// Construct the settings menu as a child of this menu
	UWorld* World = GetWorld();
	if (!World) return;
	UBLPUWSettingsMenu* SettingsMenu = CreateWidget<UBLPUWSettingsMenu>(World, SettingsMenuClass);
	if (!SettingsMenu) return;
	SettingsMenu->SetParent(this);
	if (!WidgetSwitcher) return;
	WidgetSwitcher->AddChild(SettingsMenu);
}

void UBLPUWPauseMenu::CloseSettingsMenu()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}

void UBLPUWPauseMenu::BackBtnClicked()
{
	Remove();
}
void UBLPUWPauseMenu::SettingsMenuBtnClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
}
void UBLPUWPauseMenu::MainMenuBtnClicked()
{
	UBLPGameInstance* BLPGameInstancePtr = GetGameInstance<UBLPGameInstance>();
	ABLPPlayerController* BLPPlayerControllerPtr = GetOwningPlayer<ABLPPlayerController>();
    
	if (!BLPGameInstancePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerState: BLPGameInstancePtr is null")); return; }
	if (!BLPPlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPPlayerState: BLPPlayerControllerPtr is null")); return; }
    	
	BLPGameInstancePtr->QuitToMainMenu(BLPPlayerControllerPtr);
}
