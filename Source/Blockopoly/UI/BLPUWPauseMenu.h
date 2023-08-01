// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BLPUserWidget.h"
#include "BLPUWPauseMenu.generated.h"

class UButton;
class UWidgetSwitcher;

UCLASS()
class BLOCKOPOLY_API UBLPUWPauseMenu : public UBLPUserWidget
{
	GENERATED_BODY()

public:
	UBLPUWPauseMenu();
	void CloseSettingsMenu();

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* SettingsMenuBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* BackBtn;

	// WBP References
	TSubclassOf<UUserWidget> SettingsMenuClass;
	
	UFUNCTION()
	void BackBtnClicked();
	UFUNCTION()
	void SettingsMenuBtnClicked();
	UFUNCTION()
	void MainMenuBtnClicked();
};
