// Fill out your copyright notice in the Description page of Project Settings.


#include "./BLPUWSaleRequest.h"
#include "../Framework/Controllers/BLPPlayerController.h"
#include "../Framework/State/BLPPlayerState.h"

#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

void UBLPUWSaleRequest::LoadData(const FPropertySaleData& SaleData)
{
	OwnerText->SetText(FText::FromString(SaleData.OwningPlayer->GetPlayerName()));
	AmountText->SetText(FText::FromString(FString::FromInt(SaleData.SalePrice)));
	PropertyNameTextBlock->SetText(FText::FromString(SaleData.PropertyToSell->GetName()));

	// Only set the color of the inner border if it's an EstateProperty
	if (const ABLPEstatePropertySpace* EstatePropertySpace = Cast<ABLPEstatePropertySpace>(SaleData.PropertyToSell))
	{
		FLinearColor PropertyColor;
		EstatePropertySpace->GetColor()->GetMaterial()->GetVectorParameterValue(TEXT("Color"), PropertyColor);
		PropertyTitleBorder->SetBrushColor(PropertyColor);
	}

	AssociatedSaleData = SaleData;
}

void UBLPUWSaleRequest::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!AcceptBtn) return;
	AcceptBtn->OnClicked.AddDynamic(this, &UBLPUWSaleRequest::AcceptBtnClicked);
	if (!DenyBtn) return;
	DenyBtn->OnClicked.AddDynamic(this, &UBLPUWSaleRequest::DenyBtnClicked);
}

void UBLPUWSaleRequest::AcceptBtnClicked()
{
	ABLPPlayerController* BLPPlayerControllerPtr = GetOwningPlayer<ABLPPlayerController>();
	if (!BLPPlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWSaleRequest: BLPPlayerControllerPtr is null")); return; }

	BLPPlayerControllerPtr->Server_SendSaleResponse(AssociatedSaleData, true);

	RemoveFromParent();
}

void UBLPUWSaleRequest::DenyBtnClicked()
{
	ABLPPlayerController* BLPPlayerControllerPtr = GetOwningPlayer<ABLPPlayerController>();
	if (!BLPPlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWSaleRequest: BLPPlayerControllerPtr is null")); return; }
    
	BLPPlayerControllerPtr->Server_SendSaleResponse(AssociatedSaleData, false);
    
	RemoveFromParent();
}