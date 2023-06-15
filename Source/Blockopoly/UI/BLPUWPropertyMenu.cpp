// Fill out your copyright notice in the Description page of Project Settings.


#include "./BLPUWPropertyMenu.h"

#include "BLPUWPropertyDetails.h"
#include "BLPUWPropertyTitle.h"
#include "../Framework/BLPPlayerController.h"
#include "../Framework/State/BLPPlayerState.h"
#include "../Items/Spaces/BLPPropertySpace.h"
#include "../Items/Spaces/BLPEstatePropertySpace.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/WrapBox.h"


UBLPUWPropertyMenu::UBLPUWPropertyMenu()
{
	// Gets reference to WBP_PropertyTitle
	const ConstructorHelpers::FClassFinder<UUserWidget> WBP_PropertyTitle(TEXT("/Game/Core/UI/WBP_PropertyTitle"));
	if (!WBP_PropertyTitle.Class) return;
	PropertyTitleClass = WBP_PropertyTitle.Class;
}

bool UBLPUWPropertyMenu::Initialize()
{
	// Call parent version of function and store result in variable
	const bool Success = Super::Initialize();
	if (!Success) return false;

	// Bind button callbacks
	if (!BackBtn) return false;
	BackBtn->OnClicked.AddDynamic(this, &UBLPUWPropertyMenu::BackBtnClicked);
	if (!BuildBtn) return false;
	BuildBtn->OnClicked.AddDynamic(this, &UBLPUWPropertyMenu::BuildBtnClicked);
	if (!MortgageBtn) return false;
	MortgageBtn->OnClicked.AddDynamic(this, &UBLPUWPropertyMenu::MortgageBtnClicked);

	// Bind functions to delegates in PlayerState
	ABLPPlayerState* PlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: PlayerStatePtr is null")); return false; }
	
	PlayerStatePtr->ItsMyTurnDelegate.AddUObject(this, &UBLPUWPropertyMenu::ItsMyTurn);
	PlayerStatePtr->ItsNotMyTurnDelegate.AddUObject(this, &UBLPUWPropertyMenu::ItsNotMyTurn);
	
	return true;
}

void UBLPUWPropertyMenu::RefreshPropertyWrapBox()
{
	PropertyWrapBox->ClearChildren();
	
	// Note: call this function whenever the property menu is opened from main hud class
	UWorld* World = GetWorld();
	if (!World) return;
	const ABLPPlayerController* PlayerController = Cast<ABLPPlayerController> (GetOwningLocalPlayer()->GetPlayerController(World));
	if (!PlayerController) return;
	const ABLPPlayerState* PlayerState = PlayerController->GetPlayerState<ABLPPlayerState>();
	if (!PlayerState) return;
	
	TArray<ABLPPropertySpace*> OwnedPropertyList = PlayerState->GetOwnedPropertyList();
	for (ABLPPropertySpace* Property : OwnedPropertyList)
	{
		UBLPUWPropertyTitle* PropertyTitle = CreateWidget<UBLPUWPropertyTitle>(World, PropertyTitleClass);
		if (!PropertyTitle) return;

		PropertyTitle->Setup(this, Property);
		PropertyTitle->PropertyNameText->SetText(FText::FromString(Property->GetName()));

		// Only set the color of the inner border if it's an EstateProperty
		if (const ABLPEstatePropertySpace* EstateProperty = Cast<ABLPEstatePropertySpace>(Property))
		{
			FLinearColor CurrColor;
			EstateProperty->GetColor()->GetMaterial()->GetVectorParameterValue(TEXT("Color"), CurrColor);

			PropertyTitle->InnerBorder->SetBrushColor(CurrColor);
		}

		PropertyWrapBox->AddChild(PropertyTitle);
	}
}

void UBLPUWPropertyMenu::RefreshPropertyDetails(ABLPPropertySpace* PropertySpace) const
{
	PropertyDetails->Refresh(PropertySpace);
}

void UBLPUWPropertyMenu::BackBtnClicked()
{
	UWidgetSwitcher* Parent = Cast<UWidgetSwitcher>(GetParent());
	if (Parent)
	{
		Parent->SetActiveWidgetIndex(0);
	}

	UE_LOG(LogTemp, Warning, TEXT("BackBtn Clicked"));
}

void UBLPUWPropertyMenu::BuildBtnClicked()
{

	UE_LOG(LogTemp, Warning, TEXT("BuildBtn Clicked1"));
}

void UBLPUWPropertyMenu::MortgageBtnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("MortgageBtn Clicked"));
}

void UBLPUWPropertyMenu::ItsMyTurn()
{
	MortgageBtn->SetVisibility(ESlateVisibility::Visible);
	BuildBtn->SetVisibility(ESlateVisibility::Visible);
}

void UBLPUWPropertyMenu::ItsNotMyTurn()
{
	MortgageBtn->SetVisibility(ESlateVisibility::Hidden);
	BuildBtn->SetVisibility(ESlateVisibility::Hidden);
}

