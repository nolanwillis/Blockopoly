// Fill out your copyright notice in the Description page of Project Settings.

#include "./BLPUWSaleResponse.h"
#include "../Framework/Controllers/BLPPlayerController.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UBLPUWSaleResponse::LoadData(const FPropertySaleData& SaleData, const bool& Status)
{
	PropertyNameTextBlock->SetText(FText::FromString(SaleData.PropertyToSell->GetName()));
	if (Status) StatusTextBlock->SetText(FText::FromString("Accepted"));
	else StatusTextBlock->SetText(FText::FromString("Rejected"));
	
	// Only set the color of the inner border if it's an EstateProperty
	if (const ABLPEstatePropertySpace* EstatePropertySpace = Cast<ABLPEstatePropertySpace>(SaleData.PropertyToSell))
	{
		FLinearColor PropertyColor;
		EstatePropertySpace->GetColor()->GetMaterial()->GetVectorParameterValue(TEXT("Color"), PropertyColor);
		PropertyTitleContainer->SetBackgroundColor(PropertyColor);
	}
}
