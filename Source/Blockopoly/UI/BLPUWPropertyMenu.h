// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./BLPUserWidget.h"
#include "Components/Button.h"
#include "BLPUWPropertyMenu.generated.h"

class UBLPUWPlayerCard;
class UTextBlock;
class UBLPUWPropertyTitle;
class ABLPPlayerState;
class ABLPGameState;
class ABLPPlayerController;
class ABLPPropertySpace;
class UButton;
class UWrapBox;
class UBLPUWPropertyDetails;
class UBLPUWGameMenu;
class UWidgetSwitcher;
class UEditableTextBox;

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API UBLPUWPropertyMenu : public UBLPUserWidget
{
	GENERATED_BODY()

public:
	UBLPUWPropertyMenu();

	UFUNCTION()
	void RefreshPropertyWrapBox();

	UFUNCTION()
	void RefreshPropertyDetails() const;

	void SetSelectedPropertySpace(ABLPPropertySpace* Value) { SelectedPropertySpace = Value; }
	
	void SetSelectedPropertyTitle(UBLPUWPropertyTitle* Value) { SelectedPropertyTitle = Value; }

	void SetPlayerToTradeWith(ABLPPlayerState* Value) { PlayerToTradeWith = Value; UE_LOG(LogTemp, Warning, TEXT("PlayerToTradeWith set!"));}

	UBLPUWPlayerCard* GetSelectedPlayerCard() { return SelectedPlayerCard; }
	void SetSelectedPlayerCard(UBLPUWPlayerCard* Value) { SelectedPlayerCard = Value; UE_LOG(LogTemp, Warning, TEXT("SelectedPlayerCard set!"));}

	UButton* GetConfirmTradeBtn() { return ConfirmSaleRequestBtn; }
	UButton* GetSellBtn() { return SellBtn; }
	UTextBlock* GetPendingSaleText() { return PendingSaleText; }

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	ABLPPropertySpace* SelectedPropertySpace;

	UPROPERTY()
	UBLPUWPropertyTitle* SelectedPropertyTitle;
	
	UPROPERTY()
	ABLPPlayerState* PlayerToTradeWith;
	
	UPROPERTY()
	UBLPUWPlayerCard* SelectedPlayerCard;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	UButton* BackBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* BuildBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* SellBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* MortgageBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* ConfirmSaleRequestBtn;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MortgageBtnText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SaleRequestErrorText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PendingSaleText;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* PropertyWrapBox;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* PlayerCardWrapBox;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* TradeAmountTextBox;

	UPROPERTY(meta = (BindWidget))
	UBLPUWPropertyDetails* PropertyDetails;

	// Reference to widget blueprints
	TSubclassOf<UUserWidget> PropertyTitleClass;
	TSubclassOf<UUserWidget> PlayerCardClass;

	UFUNCTION()
	void BackBtnClicked();
	
	UFUNCTION()
	void BuildBtnClicked();

	UFUNCTION()
	void MortgageBtnClicked();

	UFUNCTION()
	void SellBtnClicked();

	UFUNCTION()
	void ConfirmTradeBtnClicked();

	UFUNCTION()
	void PlayerUpId();

	UFUNCTION()
	void RefreshPlayerTradeList();

};
