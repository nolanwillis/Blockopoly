// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BLPUserWidget.h"
#include "BLPUWPauseMenu.generated.h"

class UButton;

UCLASS()
class BLOCKOPOLY_API UBLPUWPauseMenu : public UBLPUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
		UButton* MainMenuBtn;
	UPROPERTY(meta = (BindWidget))
		UButton* BackBtn;
	UFUNCTION()
		void BackBtnClicked();
	UFUNCTION()
		void MainMenuBtnClicked();
};
