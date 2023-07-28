// Fill out your copyright notice in the Description page of Project Settings.

#include "./BLPUWPropertyMenu.h"
#include "BLPUWPlayerCard.h"
#include "BLPUWPropertyDetails.h"
#include "BLPUWPropertyTitle.h"
#include "../Framework/Controllers/BLPPlayerController.h"
#include "../Framework/State/BLPPlayerState.h"
#include "../Framework/State/BLPGameState.h"
#include "../Items/Spaces/BLPPropertySpace.h"
#include "../Items/Spaces/BLPEstatePropertySpace.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/WrapBox.h"
#include "Kismet/KismetStringLibrary.h"
#include "Misc/CString.h"

UBLPUWPropertyMenu::UBLPUWPropertyMenu()
{
	// Gets reference to WBP_PropertyTitle
	const ConstructorHelpers::FClassFinder<UUserWidget> WBP_PropertyTitle(TEXT("/Game/Core/UI/WBP_PropertyTitle"));
	if (!WBP_PropertyTitle.Class) return;
	PropertyTitleClass = WBP_PropertyTitle.Class;

	// Gets reference to WBP_PlayerCard
	const ConstructorHelpers::FClassFinder<UUserWidget> WBP_PlayerCard(TEXT("/Game/Core/UI/WBP_PlayerCard"));
	if (!WBP_PlayerCard.Class) return;
	PlayerCardClass = WBP_PlayerCard.Class;
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
	if (!SellBtn) return;
	SellBtn->OnClicked.AddDynamic(this, &UBLPUWPropertyMenu::SellBtnClicked);
	if (!ConfirmSaleRequestBtn) return;
	ConfirmSaleRequestBtn->OnClicked.AddDynamic(this, &UBLPUWPropertyMenu::ConfirmTradeBtnClicked);

	ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
	if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: BLPPlayerStatePtr is null")); return; }

	BLPPlayerStatePtr->PlayerUpIdDelegate.AddUObject(this, &UBLPUWPropertyMenu::PlayerUpId);
	BLPPlayerStatePtr->OnBalanceChangedDelegate.AddUObject(this, &UBLPUWPropertyMenu::UpdateBalance);
	BLPPlayerStatePtr->RefreshUIDelegate.AddUObject(this, &UBLPUWPropertyMenu::RefreshPlayerSellList);
	BLPPlayerStatePtr->OnPropertyListChangedDelegate.BindUObject(this, &UBLPUWPropertyMenu::RefreshPropertyWrapBox);

	// Required because this menu is loaded after the initial firing of the RefreshUIDelegate 
	RefreshPlayerSellList();

	// For testing
	PropertyWrapBox->ClearChildren();
}

void UBLPUWPropertyMenu::BackBtnClicked()
{
	// If on sell submenu go back to the property list
	if (WidgetSwitcher->GetActiveWidgetIndex() == 1)
	{
		// Reset sell submenu 
		if (SelectedPlayerCard)
		{
			SelectedPlayerCard->SetRenderOpacity(0.2f);
			SelectedPlayerCard = nullptr;
			PlayerToTradeWith = nullptr;
			SaleRequestErrorText->SetVisibility(ESlateVisibility::Hidden);
		}
		WidgetSwitcher->SetActiveWidgetIndex(0);
	}
	// If on property list, exit property menu entirely
	else
	{
		UWidgetSwitcher* Parent = Cast<UWidgetSwitcher>(GetParent());
		if (Parent) Parent->SetActiveWidgetIndex(0);
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
	RefreshPropertyManagementButtons();
}
void UBLPUWPropertyMenu::SellBtnClicked()
{
	if (!SelectedPropertySpace) return;
	if (!SelectedPropertySpace->GetHasPendingSale())
	{
		WidgetSwitcher->SetActiveWidgetIndex(1);
		ConfirmSaleRequestBtn->SetVisibility(ESlateVisibility::Visible);
	}
}
void UBLPUWPropertyMenu::ConfirmTradeBtnClicked()
{
	ABLPPlayerController* BLPPlayerControllerPtr = GetOwningPlayer<ABLPPlayerController>();
	if (!BLPPlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWPropertyMenu: BLPPlayerControllerPtr is null")); return; }
	if (!SelectedPropertySpace) { UE_LOG(LogTemp, Warning, TEXT("BLPUWPropertyMenu: SelectedPropertySpace is null")); return; }
	const FString RequestedAmount = TradeAmountTextBox->GetText().ToString();

	if (RequestedAmount.IsEmpty() || !PlayerToTradeWith)
	{
		SaleRequestErrorText->SetText(FText::FromString("Select a player and enter an amount"));
		SaleRequestErrorText->SetVisibility(ESlateVisibility::Visible);
		return;
	}

	if (SelectedPropertySpace->GetHasPendingSale())
	{
		SaleRequestErrorText->SetText(FText::FromString("Property already has a pending trade"));
		SaleRequestErrorText->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	
	// Server RPC to send notification to player selected to trade
	FPropertySaleData SaleData;
	SaleData.OwningPlayer = GetOwningPlayerState<ABLPPlayerState>();
	SaleData.TargetPlayer = PlayerToTradeWith;
	SaleData.SalePrice = UKismetStringLibrary::Conv_StringToInt(RequestedAmount);
	SaleData.PropertyToSell = SelectedPropertySpace;
	BLPPlayerControllerPtr->Server_SendSaleRequest(SaleData);
	
	RefreshPropertyManagementButtons();

	// Return to property list after a sale request is sent
	WidgetSwitcher->SetActiveWidgetIndex(0);
	
	UE_LOG(LogTemp, Warning, TEXT("ConfirmTradeBtnClicked"));
}

void UBLPUWPropertyMenu::UpdateBalance(const int NewBalance)
{
	BalanceText->SetText(FText::AsNumber(NewBalance));
}
void UBLPUWPropertyMenu::PlayerUpId()
{
	RefreshPropertyManagementButtons();
}

void UBLPUWPropertyMenu::RefreshPropertyManagementButtons()
{
	const ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
    if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWNotification: BLPPlayerStatePtr is null")); return; }
	
    if (BLPPlayerStatePtr->GetPlayerUpId() == BLPPlayerStatePtr->GetBLPPlayerId())
    {
		if (!SelectedPropertySpace)
		{
			MortgageBtn->SetVisibility(ESlateVisibility::Collapsed);
			BuildBtn->SetVisibility(ESlateVisibility::Collapsed);
			SellBtn->SetVisibility(ESlateVisibility::Collapsed);
			PendingSaleText->SetVisibility(ESlateVisibility::Hidden);
			return;
		}

    	if (SelectedPropertySpace->GetIsMortgaged())
    	{
    		BuildBtn->SetVisibility(ESlateVisibility::Collapsed);
    		SellBtn->SetVisibility(ESlateVisibility::Collapsed);
            return;			
    	}
    	
    	if (SelectedPropertySpace->GetHasPendingSale())
    	{
    		MortgageBtn->SetVisibility(ESlateVisibility::Collapsed);
        	BuildBtn->SetVisibility(ESlateVisibility::Collapsed);
    		SellBtn->SetVisibility(ESlateVisibility::Collapsed);
    		PendingSaleText->SetVisibility(ESlateVisibility::Visible);
    	}
    	else
    	{
    		PendingSaleText->SetVisibility(ESlateVisibility::Hidden);
    		MortgageBtn->SetVisibility(ESlateVisibility::Visible);
    		SellBtn->SetVisibility(ESlateVisibility::Visible);
    		if (const ABLPEstatePropertySpace* EstatePropertySpacePtr = Cast<ABLPEstatePropertySpace>(SelectedPropertySpace))
    		{
    			if (EstatePropertySpacePtr->GetCanBuild()) BuildBtn->SetVisibility(ESlateVisibility::Visible);
    		}
    	}
    }
    else
    {
    	MortgageBtn->SetVisibility(ESlateVisibility::Collapsed);
        BuildBtn->SetVisibility(ESlateVisibility::Collapsed);
    	SellBtn->SetVisibility(ESlateVisibility::Collapsed);
    	if (!SelectedPropertySpace) return;
    	if (SelectedPropertySpace->GetHasPendingSale()) PendingSaleText->SetVisibility(ESlateVisibility::Visible);
    	else PendingSaleText->SetVisibility(ESlateVisibility::Hidden);
    }
}
void UBLPUWPropertyMenu::ResetPropertyDetails()
{
	PropertyDetails->Reset();
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
		PropertyTitle->PropertyNameTextBlock->SetText(FText::FromString(Property->GetName()));

		// Only set the color of the inner border if it's an EstateProperty
		if (const ABLPEstatePropertySpace* EstatePropertySpace = Cast<ABLPEstatePropertySpace>(Property))
		{
			FLinearColor CurrColor;
			EstatePropertySpace->GetColor()->GetMaterial()->GetVectorParameterValue(TEXT("Color"), CurrColor);

			PropertyTitle->PropertyTitleBorder->SetBrushColor(CurrColor);
		}
		
		PropertyWrapBox->AddChild(PropertyTitle);
	}
}
void UBLPUWPropertyMenu::RefreshPlayerSellList()
{
	PlayerCardWrapBox->ClearChildren();
    	
	UWorld* World = GetWorld();
	if (!World) return;
	const ABLPGameState* BLPGameStatePtr = World->GetGameState<ABLPGameState>();
	if (!BLPGameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: BLPGameStatePtr is null ")); return;}
	
	const TArray<TObjectPtr<APlayerState>> PlayerArray = BLPGameStatePtr->PlayerArray;
    	
	for (APlayerState* PlayerState : PlayerArray)
	{
		ABLPPlayerState* BLPPlayerState = Cast<ABLPPlayerState>(PlayerState);
		if (!BLPPlayerState) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: BLPPlayerStatePtr is null ")); return;}

		if (BLPPlayerState->GetIsLeaving()) return;
		
		// Don't add forfeited players 
		TArray<int> ForfeitedPlayersArray = BLPGameStatePtr->GetForfeitedPlayersArray();
		if (ForfeitedPlayersArray.Contains(BLPPlayerState->GetBLPPlayerId())) continue;

		// Don't add self
		if (BLPPlayerState->GetBLPPlayerId() == GetOwningPlayerState<ABLPPlayerState>()->GetBLPPlayerId()) continue;

		UBLPUWPlayerCard* PlayerCard = CreateWidget<UBLPUWPlayerCard>(World, PlayerCardClass);
		if (!PlayerCard) return;

		// Setup function is required in this player list so the property menu knows which
		// player card and associated player state is selected.
		PlayerCard->Setup(this, BLPPlayerState);
		PlayerCard->PlayerNameText->SetText(FText::FromString(BLPPlayerState->GetPlayerName()));

		PlayerCard->SetRenderOpacity(0.2f);
    		
		switch(BLPPlayerState->GetBLPPlayerId())
		{
		case 0:
			PlayerCard->Container->SetBackgroundColor(FLinearColor(1, .1588, 0, .8));
			break;
		case 1:
			PlayerCard->Container->SetBackgroundColor( FLinearColor(.645, 0, 1, .8));
			break;			
		case 2:
			PlayerCard->Container->SetBackgroundColor( FLinearColor(.0243, .566, 0, .8));
			break;			
		case 3:
			PlayerCard->Container->SetBackgroundColor( FLinearColor(.0, .286, 1, .8));
			break;
		default:
			PlayerCard->Container->SetBackgroundColor( FLinearColor(1, 1, 1, 1));
			break;
		}
		
		PlayerCardWrapBox->AddChild(PlayerCard);
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



