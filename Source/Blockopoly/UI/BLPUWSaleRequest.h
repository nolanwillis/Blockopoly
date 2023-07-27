// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./BLPUserWidget.h"
#include "Blockopoly/Framework/Controllers/BLPPlayerController.h"
#include "BLPUWSaleRequest.generated.h"

struct FPropertySaleData;
class UButton;
class UTextBlock;
class UBorder;

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API UBLPUWSaleRequest : public UBLPUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void LoadData(const FPropertySaleData& SaleData);

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* OwnerText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmountText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PropertyNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	UBorder* PropertyTitleBorder;
	
	UPROPERTY(meta = (BindWidget))
	UButton* AcceptBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* DenyBtn;

	FPropertySaleData AssociatedSaleData = {nullptr, nullptr, nullptr, 0};

	UFUNCTION()
	void AcceptBtnClicked();

	UFUNCTION()
	void DenyBtnClicked();
};