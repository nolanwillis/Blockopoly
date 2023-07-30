// Fill out your copyright notice in the Description page of Project Settings.


#include "./BLPUWSettingsMenu.h"
#include "./BLPUWMainMenu.h"
#include "Components/Button.h"

void UBLPUWSettingsMenu::NativeConstruct()
{
	Super::NativeConstruct();
	if (!BackBtn) return;
	BackBtn->OnClicked.AddDynamic(this, &UBLPUWSettingsMenu::BackBtnClicked);
}

void UBLPUWSettingsMenu::BackBtnClicked()
{
	if (!Parent) return;
	Parent->CloseSettingsMenu();
}

void UBLPUWSettingsMenu::ApplyBtnClicked()
{
}
