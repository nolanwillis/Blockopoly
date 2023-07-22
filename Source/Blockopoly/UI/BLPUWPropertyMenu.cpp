// Fill out your copyright notice in the Description page of Project Settings.


#include "./BLPUWPropertyMenu.h"

#include "BLPUWPropertyDetails.h"
#include "BLPUWPropertyTitle.h"
#include "../Framework/Controllers/BLPPlayerController.h"
#include "../Framework/State/BLPPlayerState.h"
#include "../Framework/State/BLPGameState.h"
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

void UBLPUWPropertyMenu::NativeConstruct()
{
	// Call parent version of function
	Super::NativeConstruct();

	// Bind button callbacks
	if (!BackBtn) return;
	BackBtn->OnClicked.AddDynamic(this, &UBLPUWPropertyMenu::BackBtnClicked);
	if (!BuildBtn) return;
	BuildBtn->OnClicked.AddDynamic(this, &UBLPUWPropertyMenu::BuildBtnClicked);
	if (!MortgageBtn) return;
	MortgageBtn->OnClicked.AddDynamic(this, &UBLPUWPropertyMenu::MortgageBtnClicked);

	ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();

	if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: BLPPlayerStatePtr is null")); return; }

	BLPPlayerStatePtr->PlayerUpIdDelegate.AddUObject(this, &UBLPUWPropertyMenu::PlayerUpId);
}

void UBLPUWPropertyMenu::RefreshPropertyWrapBox()
{
	PropertyWrapBox->ClearChildren();

	UWorld* WorldPtr = GetWorld();
	const ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();

	if (!WorldPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWPropertyMenu: World is null")); return; }
	if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: BLPPlayerStatePtr is null")); return; }
	
	TArray<ABLPPropertySpace*> OwnedPropertyList = BLPPlayerStatePtr->GetOwnedPropertyList();
	for (ABLPPropertySpace* Property : OwnedPropertyList)
	{
		UBLPUWPropertyTitle* PropertyTitle = CreateWidget<UBLPUWPropertyTitle>(WorldPtr, PropertyTitleClass);
		if (!PropertyTitle) return;

		PropertyTitle->Setup(this, Property);
		PropertyTitle->PropertyNameText->SetText(FText::FromString(Property->GetName()));

		// Only set the color of the inner border if it's an EstateProperty
		if (const ABLPEstatePropertySpace* EstatePropertySpace = Cast<ABLPEstatePropertySpace>(Property))
		{
			FLinearColor CurrColor;
			EstatePropertySpace->GetColor()->GetMaterial()->GetVectorParameterValue(TEXT("Color"), CurrColor);

			PropertyTitle->InnerBorder->SetBrushColor(CurrColor);
		}

		PropertyWrapBox->AddChild(PropertyTitle);
	}
}

void UBLPUWPropertyMenu::RefreshPropertyDetails() const
{
	const ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();

	if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: BLPPlayerStatePtr is null")); return; }

	const bool IsItOwnersTurn = BLPPlayerStatePtr->GetPlayerUpId() == BLPPlayerStatePtr->GetBLPPlayerId();
	
	if (!SelectedPropertySpace) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: SelectedPropertySpace is null")); return; }
	PropertyDetails->Refresh(IsItOwnersTurn, SelectedPropertySpace, BuildBtn, MortgageBtnText);
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
	const UWorld* World = GetWorld();
	if (!World) { UE_LOG(LogTemp, Warning, TEXT("BLPUWPropertyMenu: World is null")); return; }
	ABLPPlayerController* BLPPlayerControllerPtr = GetOwningPlayer<ABLPPlayerController>();
	ABLPGameState* BLPGameStatePtr = World->GetGameState<ABLPGameState>();
	ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();

	if (!BLPPlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: BLPPlayerControllerPtr is null")); return; }
	if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: BLPPlayerStatePtr is null")); return; }
	if (!BLPGameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: BLPGameStatePtr is null")); return; }	

	if (!BLPPlayerStatePtr->GetPlayerUpId() == BLPPlayerStatePtr->GetBLPPlayerId()) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: It's not your turn")); return; }

	if (const ABLPEstatePropertySpace* EstatePropertySpacePtr = Cast<ABLPEstatePropertySpace>(SelectedPropertySpace))
	{
		if (!EstatePropertySpacePtr->GetCanBuild()) { UE_LOG(LogTemp, Warning, TEXT("BLPUWPropertyMenu: You're not allowed to build on the selected estate property")); return;}
		
		BLPPlayerControllerPtr->Server_BuyBuilding(BLPPlayerStatePtr, BLPGameStatePtr, EstatePropertySpacePtr->GetSpaceID());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: Estate property not selected"));
	}
	UE_LOG(LogTemp, Warning, TEXT("BuildBtn Clicked1"));
}

void UBLPUWPropertyMenu::MortgageBtnClicked()
{
	UWorld* WorldPtr = GetWorld();
	if (!WorldPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWPropertyMenu: World is null")); return; }
	ABLPPlayerController* BLPPlayerControllerPtr = GetOwningPlayer<ABLPPlayerController>();
	ABLPGameState* BLPGameStatePtr = WorldPtr->GetGameState<ABLPGameState>();
	ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();

	if (!BLPPlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: BLPPlayerControllerPtr is null")); return; }
	if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: BLPPlayerStatePtr is null")); return; }
	if (!BLPGameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: BLPGameStatePtr is null")); return; }	

	if (!BLPPlayerStatePtr->GetPlayerUpId() == BLPPlayerStatePtr->GetBLPPlayerId()) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: It's not your turn")); return; }

	if (!SelectedPropertySpace) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: No property was selected")); return; }

	// Simulate locally
	if (SelectedPropertySpace->GetIsMortgaged())
	{
		SelectedPropertySpace->SetIsMortgaged(false);
		BLPPlayerControllerPtr->Server_SetMortgageStatus(BLPPlayerStatePtr, BLPGameStatePtr, SelectedPropertySpace->GetSpaceID(), false);
	}
	else
	{
		SelectedPropertySpace->SetIsMortgaged(true);
		BLPPlayerControllerPtr->Server_SetMortgageStatus(BLPPlayerStatePtr, BLPGameStatePtr, SelectedPropertySpace->GetSpaceID(), true);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("MortgageBtn Clicked"));

	RefreshPropertyDetails();
}

void UBLPUWPropertyMenu::PlayerUpId()
{
	const ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
	if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWNotification: BLPPlayerStatePtr is null")); return; }

	// Its the owners turn
	if (BLPPlayerStatePtr->GetPlayerUpId() == BLPPlayerStatePtr->GetBLPPlayerId())
	{
		MortgageBtn->SetVisibility(ESlateVisibility::Visible);
		if (const ABLPEstatePropertySpace* EstatePropertySpacePtr = Cast<ABLPEstatePropertySpace>(SelectedPropertySpace))
		{
			if (EstatePropertySpacePtr->GetCanBuild())
			{
				BuildBtn->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
	// Its not the owners turn
	else
	{
		MortgageBtn->SetVisibility(ESlateVisibility::Hidden);
    	BuildBtn->SetVisibility(ESlateVisibility::Hidden);
	}
}
