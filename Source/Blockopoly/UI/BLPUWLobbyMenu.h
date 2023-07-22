// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./BLPUserWidget.h"
#include "BLPUWLobbyMenu.generated.h"

class UButton;
class UWrapBox;
class UTextBlock;
class UWidgetSwitcher;

UCLASS()
class BLOCKOPOLY_API UBLPUWLobbyMenu : public UBLPUserWidget
{
	GENERATED_BODY()

public:
	UBLPUWLobbyMenu();

	UFUNCTION()
	void Refresh();

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta=(BindWidget))
	UButton* ReadyBtn;

	UPROPERTY(meta=(BindWidget))
	UButton* PlayBtn;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ReadyBtnTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WaitingTextBlock;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* PlayerCardWrapBox;
	
	UPROPERTY(meta=(BindWidget))
	UWidgetSwitcher* WidgetSwitcher;
    	
	UFUNCTION()
	void ReadyBtnClicked();

	UFUNCTION()
	void PlayBtnClicked();


	UFUNCTION()
	void RefreshPlayerList();

	UFUNCTION()
	void CheckAllReadyStatus();

	TSubclassOf<UUserWidget> PlayerCardClass;
};
