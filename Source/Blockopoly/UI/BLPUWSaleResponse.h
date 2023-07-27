// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./BLPUserWidget.h"
#include "BLPUWSaleResponse.generated.h"

struct FPropertySaleData;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API UBLPUWSaleResponse : public UBLPUserWidget
{
	GENERATED_BODY()

public:
	void LoadData(const FPropertySaleData& SaleData, const bool& Status);
    
private:
    UPROPERTY(meta =(BindWidget))
    UTextBlock* StatusTextBlock;
    UPROPERTY(meta =(BindWidget))
    UTextBlock* PropertyNameTextBlock;
    
    UPROPERTY(meta =(BindWidget))
    UButton* PropertyTitleContainer;
};
