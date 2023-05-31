// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPUserWidget.h"

// Adds a UI element to the viewport
void UBLPUserWidget::Setup()
{
	this->AddToViewport();
	UWorld* World = GetWorld();
	if (!World) return;
	// Gets reference to player controller
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController) return;

	// Settings that enable a user to interace with the menu via mouse
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

// Removes a UI element from the viewport
void UBLPUserWidget::Remove()
{
	this->RemoveFromViewport();
	UWorld* World = GetWorld();
	if (!World) return;
	// Gets reference to player controller
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController) return;

	// Settings that enable a user to interace with the menu via mouse
	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}
