// Fill out your copyright notice in the Description page of Project Settings.


#include "./BLPUWPropertyDetails.h"
#include "../Items/Spaces/BLPPropertySpace.h"
#include "../Items/Spaces/BLPEstatePropertySpace.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

void UBLPUWPropertyDetails::Refresh(ABLPPropertySpace* PropertySpace) const
{
	
	
	PropertyNameText->SetText(FText::FromString(PropertySpace->GetName()));
	Rent0Text->SetText(FText::AsNumber(PropertySpace->GetRent()));
	MortgageText->SetText(FText::AsNumber(PropertySpace->GetMortgageValue()));

	if (const ABLPEstatePropertySpace* EstateProperty = Cast<ABLPEstatePropertySpace>(PropertySpace))
	{
		FLinearColor CurrColor;
		EstateProperty->GetColor()->GetMaterial()->GetVectorParameterValue(TEXT("Color"), CurrColor);
		InnerBorder->SetBrushColor(CurrColor);
		
		SetRentText->SetText(FText::AsNumber(EstateProperty->GetSetRent()));
		Rent1Text->SetText(FText::AsNumber(EstateProperty->GetRent1Houses()));
		Rent2Text->SetText(FText::AsNumber(EstateProperty->GetRent2Houses()));
		Rent3Text->SetText(FText::AsNumber(EstateProperty->GetRent3Houses()));
		Rent4Text->SetText(FText::AsNumber(EstateProperty->GetRent4Houses()));
		Rent5Text->SetText(FText::AsNumber(EstateProperty->GetRentHotel()));
		BuildingCostText->SetText(FText::AsNumber(EstateProperty->GetBuildingCost()));
	}
	else
	{
		InnerBorder->SetBrushColor(FLinearColor(1,1,1));
	}
}
