// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./BLPUserWidget.h"
#include "BLPUWGameMenu.generated.h"

class UWrapBox;
class USizeBox;
class UButton;
class UTextBlock;
class UWidgetSwitcher;
class UBLPUWPropertyMenu;
class UImage;
class UWrapBox;
class ABLPPlayerState;
class ABLPGameState;

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API UBLPUWGameMenu : public UBLPUserWidget
{
	GENERATED_BODY()

public:
	UBLPUWGameMenu();
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MainWidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* LargeNotificationWidgetSwitcher;

	UFUNCTION()
	void RefreshPlayerList();

	UFUNCTION()
	void AddNotification(const FString& Type, const FString& Heading, const FString& Description);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UBLPUWPropertyMenu* PropertyMenu;
	
	UPROPERTY(meta = (BindWidget))
	UButton* PropertyMenuBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* BuyBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* RollBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* FinishTurnBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* SkipJailBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* ForfeitBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* JailSkipCounterContainer;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* YourTurnText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InJailText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* JailSkipCounterTextBlock;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BalanceText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BankruptText; 

	UPROPERTY(meta = (BindWidget))
	UImage* InJailImage;
	
	UPROPERTY(meta = (BindWidget))
	UImage* JailSkipCounterImage;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* PlayerCardWrapBox;

	UPROPERTY(meta = (BindWidget))
	USizeBox* CardNotificationSlot;

	UPROPERTY(meta = (BindWidget))
	USizeBox* BannerNotificationSlot;
	
	// References to widget blueprints
	TSubclassOf<UUserWidget> PlayerCardClass;
	TSubclassOf<UUserWidget> ChanceCardNotificationClass;
	TSubclassOf<UUserWidget> ChestCardNotificationClass;
	TSubclassOf<UUserWidget> RollNotificationClass;
	TSubclassOf<UUserWidget> ForfeitNotificationClass;
	TSubclassOf<UUserWidget> LeaveNotificationClass;

	// Button handlers
	UFUNCTION()
	void PropertyMenuBtnClicked();
	UFUNCTION()
	void BuyBtnClicked();
	UFUNCTION()
	void RollBtnClicked();
	UFUNCTION()
	void FinishTurnBtnClicked();
	UFUNCTION()
	void SkipJailBtnClicked();
	UFUNCTION()
	void ForfeitBtnClicked();

	// Functions bound to delegates in PlayerState
	UFUNCTION()
	void PlayerUpId();
	UFUNCTION()
	void UpdateBalance(const int NewBalance);
	UFUNCTION()
	void InJail(const int TurnsLeft);
	UFUNCTION()
	void OutOfJail();
	UFUNCTION()
	void CanBuy(const bool Value);
	UFUNCTION()
	void HasRolled(const bool Value);
	UFUNCTION()
	void UpdateJailSkipCounter(const int& JailSkipCounter);
	UFUNCTION()
	void CheckBankruptcyStatus() const;

};
