// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./BLPUserWidget.h"
#include "Components/Button.h"
#include "BLPUWPropertyDetails.generated.h"

class ABLPPropertySpace;
class UTextBlock;
class UButton;
class UBorder;
class UWidgetSwitcher;

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API UBLPUWPropertyDetails : public UBLPUserWidget
{
	GENERATED_BODY()

public:
	void Refresh(const bool& IsItMyTurn, ABLPPropertySpace* PropertySpace, UButton* BuildBtn, UTextBlock* MortgageBtnText) const;
	void Reset();
	
private:
	UPROPERTY(meta = (BindWidget))
	UButton* TitleContainer;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PropertyNameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentRentText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BaseRentText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Rent1Text;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Rent2Text;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Rent3Text;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Rent4Text;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Rent5Text;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MortgagePriceText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MortgageDescText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BuildingCostText;
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;
	
};
