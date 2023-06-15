// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./BLPUserWidget.h"
#include "BLPUWPropertyDetails.generated.h"

class ABLPPropertySpace;
class UTextBlock;
class UBorder;

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API UBLPUWPropertyDetails : public UBLPUserWidget
{
	GENERATED_BODY()

public:
	void Refresh(ABLPPropertySpace* PropertySpace) const;
	
private:
	UPROPERTY(meta = (BindWidget))
	UBorder* InnerBorder;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PropertyNameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Rent0Text;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SetRentText;
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
	UTextBlock* MortgageText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BuildingCostText;
	
};
