// Fill out your copyright notice in the Description page of Project Settings.


#include "./BLPUWPropertyDetails.h"
#include "../Items/Spaces/BLPPropertySpace.h"
#include "../Items/Spaces/BLPEstatePropertySpace.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void UBLPUWPropertyDetails::Refresh(const bool& IsItMyTurn, ABLPPropertySpace* PropertySpace, UButton* BuildBtn, UTextBlock* MortgageBtnText) const
{
	// When selecting a new title, always ensure the build button is hidden
	BuildBtn->SetVisibility(ESlateVisibility::Hidden);
	
	PropertyNameText->SetText(FText::FromString(PropertySpace->GetName()));
	CurrentRentText->SetText(FText::AsNumber(PropertySpace->GetCurrentRent()));
	BaseRentText->SetText(FText::AsNumber(PropertySpace->GetBaseRent()));
	MortgagePriceText->SetText(FText::AsNumber(PropertySpace->GetMortgageValue()));
	const FString MortgageDescription = "Pay " + FString::FromInt(PropertySpace->GetMortgageValue()) + "M to reacquire";
	MortgageDescText->SetText(FText::FromString(MortgageDescription));

	if (const ABLPEstatePropertySpace* EstatePropertySpace = Cast<ABLPEstatePropertySpace>(PropertySpace))
	{
		FLinearColor CurrColor;
		EstatePropertySpace->GetColor()->GetMaterial()->GetVectorParameterValue(TEXT("Color"), CurrColor);
		InnerBorder->SetBrushColor(CurrColor);
		Rent1Text->SetText(FText::AsNumber(EstatePropertySpace->GetRent1Houses()));
		Rent2Text->SetText(FText::AsNumber(EstatePropertySpace->GetRent2Houses()));
		Rent3Text->SetText(FText::AsNumber(EstatePropertySpace->GetRent3Houses()));
		Rent4Text->SetText(FText::AsNumber(EstatePropertySpace->GetRent4Houses()));
		Rent5Text->SetText(FText::AsNumber(EstatePropertySpace->GetRentHotel()));
		BuildingCostText->SetText(FText::AsNumber(EstatePropertySpace->GetBuildingCost()));
		
		if (EstatePropertySpace->GetCanBuild() && IsItMyTurn)
		{
			BuildBtn->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		InnerBorder->SetBrushColor(FLinearColor(1,1,1));
	}

	if (PropertySpace->GetIsMortgaged())
	{
		WidgetSwitcher->SetActiveWidgetIndex(1);
		MortgageBtnText->SetText(FText::FromString("Reacquire"));
	}
	else
	{
		WidgetSwitcher->SetActiveWidgetIndex(0);
		MortgageBtnText->SetText(FText::FromString("Mortgage"));
	}
}
