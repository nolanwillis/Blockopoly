// Fill out your copyright notice in the Description page of Project Settings.


#include "./BLPUWPropertyDetails.h"
#include "../Items/Spaces/BLPPropertySpace.h"
#include "../Items/Spaces/BLPEstatePropertySpace.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void UBLPUWPropertyDetails::Refresh(const bool& IsItMyTurn, ABLPPropertySpace* PropertySpace, UButton* BuildBtn, UTextBlock* MortgageBtnText) const
{
	PropertyNameText->SetText(FText::FromString(PropertySpace->GetName()));
	CurrentRentText->SetText(FText::AsNumber(PropertySpace->GetCurrentRent()));
	BaseRentText->SetText(FText::AsNumber(PropertySpace->GetBaseRent()));
	MortgagePriceText->SetText(FText::AsNumber(PropertySpace->GetMortgageValue()));
	const FString MortgageDescription = "Pay $" + FString::FromInt(PropertySpace->GetMortgageValue()) + " to reacquire";
	MortgageDescText->SetText(FText::FromString(MortgageDescription));

	if (const ABLPEstatePropertySpace* EstatePropertySpace = Cast<ABLPEstatePropertySpace>(PropertySpace))
	{
		FLinearColor CurrColor;
		EstatePropertySpace->GetColor()->GetMaterial()->GetVectorParameterValue(TEXT("Color"), CurrColor);
		TitleContainer->SetBackgroundColor(CurrColor);
		Rent1Text->SetText(FText::AsNumber(EstatePropertySpace->GetRent1Houses()));
		Rent2Text->SetText(FText::AsNumber(EstatePropertySpace->GetRent2Houses()));
		Rent3Text->SetText(FText::AsNumber(EstatePropertySpace->GetRent3Houses()));
		Rent4Text->SetText(FText::AsNumber(EstatePropertySpace->GetRent4Houses()));
		Rent5Text->SetText(FText::AsNumber(EstatePropertySpace->GetRentHotel()));
		BuildingCostText->SetText(FText::AsNumber(EstatePropertySpace->GetBuildingCost()));
	}
	else
	{
		TitleContainer->SetBackgroundColor(FLinearColor(1,1,1));
		Rent1Text->SetText(FText::FromString("na"));
		Rent2Text->SetText(FText::FromString("na"));
		Rent3Text->SetText(FText::FromString("na"));
		Rent4Text->SetText(FText::FromString("na"));
		Rent5Text->SetText(FText::FromString("na"));
		BuildingCostText->SetText(FText::FromString("na"));
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

void UBLPUWPropertyDetails::Reset()
{
	TitleContainer->SetBackgroundColor(FLinearColor(1,1,1, 1));
	PropertyNameText->SetText(FText::FromString("Select a property"));
	CurrentRentText->SetText(FText::FromString(""));
	BaseRentText->SetText(FText::FromString(""));
	Rent1Text->SetText(FText::FromString(""));
	Rent2Text->SetText(FText::FromString(""));
	Rent3Text->SetText(FText::FromString(""));
	Rent4Text->SetText(FText::FromString(""));
	Rent5Text->SetText(FText::FromString(""));
	MortgagePriceText->SetText(FText::FromString(""));
	BuildingCostText->SetText(FText::FromString(""));
}
