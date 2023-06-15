// Fill out your copyright notice in the Description page of Project Settings.

#include "BLPUWGameMenu.h"

#include "BLPUWDrawCardMessage.h"
#include "BLPUWPropertyMenu.h"
#include "BLPUWPlayerCard.h"
#include "../Framework/Pawns/BLPAvatar.h"
#include "../Framework/BLPPlayerController.h"
#include "../Framework/State/BLPGameState.h"
#include "../Framework/State/BLPPlayerState.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/WrapBox.h"

UBLPUWGameMenu::UBLPUWGameMenu()
{
	// Gets reference to WBP_PropertyTitle
	const ConstructorHelpers::FClassFinder<UUserWidget> WBP_PlayerCard(TEXT("/Game/Core/UI/WBP_PlayerCard"));
	if (!WBP_PlayerCard.Class) return;
	PlayerCardClass = WBP_PlayerCard.Class;

	const ConstructorHelpers::FClassFinder<UUserWidget> WBP_DrawChanceCardMessage(TEXT("/Game/Core/UI/WBP_DrawChanceCardMessage"));
	if (!WBP_DrawChanceCardMessage.Class) return;
	DrawChanceCardMessageClass = WBP_DrawChanceCardMessage.Class;

	const ConstructorHelpers::FClassFinder<UUserWidget> WBP_DrawChestCardMessage(TEXT("/Game/Core/UI/WBP_DrawChestCardMessage"));
	if (!WBP_DrawChestCardMessage.Class) return;
	DrawChestCardMessageClass = WBP_DrawChestCardMessage.Class;
}

bool UBLPUWGameMenu::Initialize()
{
	// Call parent version of function and store result in variable
	const bool Success = Super::Initialize();
	if (!Success) return false;

	// Bind button callbacks to OnClicked delegate
	if (!PropertyMenuBtn) return false;
	PropertyMenuBtn->OnClicked.AddDynamic(this, &UBLPUWGameMenu::PropertyMenuBtnClicked);
	if (!BuyBtn) return false;
	BuyBtn->OnClicked.AddDynamic(this, &UBLPUWGameMenu::BuyBtnClicked);
	if (!RollBtn) return false;
	RollBtn->OnClicked.AddDynamic(this, &UBLPUWGameMenu::RollBtnClicked);
	if (!FinishTurnBtn) return false;
	FinishTurnBtn->OnClicked.AddDynamic(this, &UBLPUWGameMenu::FinishTurnBtnClicked);

	ABLPPlayerController* PlayerControllerPtr = GetOwningPlayer<ABLPPlayerController>();
	if (!PlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: PlayerControllerPtr is null")); return false; }
	
	// Bind functions to delegates in PlayerState
	ABLPPlayerState* PlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: PlayerStatePtr is null")); return false; }
	
	PlayerStatePtr->ItsMyTurnDelegate.AddUObject(this, &UBLPUWGameMenu::ItsMyTurn);
	PlayerStatePtr->ItsNotMyTurnDelegate.AddUObject(this, &UBLPUWGameMenu::ItsNotMyTurn);
	PlayerStatePtr->OnBalanceChangedDelegate.AddUObject(this, &UBLPUWGameMenu::UpdateBalance);
	PlayerStatePtr->InJailDelegate.BindUObject(this, &UBLPUWGameMenu::InJail);
	PlayerStatePtr->OutOfJailDelegate.BindUObject(this, &UBLPUWGameMenu::OutOfJail);
	PlayerStatePtr->PlayerCountDelegate.BindUObject(this, &UBLPUWGameMenu::RefreshPlayerList);
	PlayerStatePtr->CanBuyDelegate.BindUObject(this, &UBLPUWGameMenu::CanBuy);
	PlayerStatePtr->HasRolledDelegate.BindUObject(this, &UBLPUWGameMenu::HasRolled);
	PlayerStatePtr->CardDrawnDelegate.BindUObject(this, &UBLPUWGameMenu::CardDrawn);

	// Required to initially setup the player list
	RefreshPlayerList();
	
	// If not on server, ItsNotMyTurn won't be called initially, since this function is only automatically
	// called when the PlayerUpIndex changes (the host will always be first).
	if (GetOwningPlayer()->GetLocalRole() < ROLE_Authority)
	{
		ItsNotMyTurn();
	}
	
	return true;
}

void UBLPUWGameMenu::PropertyMenuBtnClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
	PropertyMenu->RefreshPropertyWrapBox();
	
	UE_LOG(LogTemp, Warning, TEXT("Property menu button clicked"));
}

void UBLPUWGameMenu::BuyBtnClicked()
{
	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(GetOwningPlayer());
	if (!PlayerControllerPtr) return;
	
	const UWorld* World = GetWorld();
	if (!World) return;
	ABLPGameState* GameStatePtr = World->GetGameState<ABLPGameState>(); 
	ABLPPlayerState* PlayerStatePtr = PlayerControllerPtr->GetPlayerState<ABLPPlayerState>();
	
	PlayerControllerPtr->Server_BuyPropertySpace(PlayerStatePtr, GameStatePtr);

	BuyBtn->SetVisibility(ESlateVisibility::Hidden);

	UE_LOG(LogTemp, Warning, TEXT("Buy button clicked"));
}

void UBLPUWGameMenu::RollBtnClicked()
{
	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(GetOwningPlayer());
	if (!PlayerControllerPtr) return;
	
	const UWorld* World = GetWorld();
	if (!World) return;
	ABLPGameState* GameStatePtr = World->GetGameState<ABLPGameState>(); 
	ABLPPlayerState* PlayerStatePtr = PlayerControllerPtr->GetPlayerState<ABLPPlayerState>();
	ABLPAvatar* AvatarPtr = PlayerControllerPtr->GetPawn<ABLPAvatar>();
	
	PlayerControllerPtr->Server_Roll(AvatarPtr, PlayerStatePtr, GameStatePtr);
	
	UE_LOG(LogTemp, Warning, TEXT("Roll button clicked"));
}

void UBLPUWGameMenu::FinishTurnBtnClicked()
{
	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(GetOwningPlayer());
	if (!PlayerControllerPtr) return;
	
	const UWorld* World = GetWorld();
	if (!World) return;
	ABLPGameState* GameStatePtr = World->GetGameState<ABLPGameState>(); 
	ABLPPlayerState* PlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
	
	PlayerControllerPtr->Server_FinishTurn(PlayerStatePtr, GameStatePtr);

	UE_LOG(LogTemp, Warning, TEXT("Finish turn button clicked"));
}

void UBLPUWGameMenu::ItsMyTurn()
{
	const ABLPPlayerState* PlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
	if (!PlayerStatePtr) UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: PlayerStatePtr is null"));
	
	YourTurnText->SetText(FText::FromString("It's your turn"));

	if (PlayerStatePtr->GetJailCounter() == 0) RollBtn->SetVisibility(ESlateVisibility::Visible);
}

void UBLPUWGameMenu::ItsNotMyTurn()
{
	const UWorld* World = GetWorld();
	if (!World) return;
	ABLPGameState* GameStatePtr = World->GetGameState<ABLPGameState>();
	if (!GameStatePtr) return;
	const int PlayerUpIndex = GameStatePtr->GetPlayerUpIndex();
	const FString PlayerUpName = GameStatePtr->PlayerArray[PlayerUpIndex]->GetPlayerName();
	
	YourTurnText->SetText(FText::FromString(PlayerUpName + " is up"));
	RollBtn->SetVisibility(ESlateVisibility::Hidden);
	FinishTurnBtn->SetVisibility(ESlateVisibility::Hidden);
	BuyBtn->SetVisibility(ESlateVisibility::Hidden);
}

void UBLPUWGameMenu::UpdateBalance(const int NewBalance)
{
	BalanceText->SetText(FText::AsNumber(NewBalance));
}

void UBLPUWGameMenu::InJail(const int TurnsLeft)
{
	FString Message = "YOUR IN JAIL FOR ";
	Message.AppendInt(TurnsLeft);
	if (TurnsLeft == 1)
	{
		Message.Append(" MORE TURN");
	}
	else
	{
		Message.Append(" MORE TURNS");
	}
	InJailText->SetText( FText::FromString(Message));
	InJailText->SetVisibility(ESlateVisibility::Visible);
	InJailImage->SetVisibility(ESlateVisibility::Visible);
}

void UBLPUWGameMenu::OutOfJail()
{
	InJailText->SetVisibility(ESlateVisibility::Hidden);
	InJailImage->SetVisibility(ESlateVisibility::Hidden);
}

void UBLPUWGameMenu::CanBuy(const bool Value)
{
	if (Value)
	{
		BuyBtn->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BuyBtn->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBLPUWGameMenu::HasRolled(const bool Value)
{
	if (Value)
	{
		UE_LOG(LogTemp, Warning, TEXT("Finish button should be visible"));
		FinishTurnBtn->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Finish button should NOT be visible"));
		FinishTurnBtn->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBLPUWGameMenu::CardDrawn(const FString& Type, const FString& Heading, const FString& Description)
{
	DrawCardMessageSlot->ClearChildren();
	
	UWorld* World = GetWorld();
	if (!World) return;

	UBLPUWDrawCardMessage* DrawCardMessage = nullptr;
	
	if (Type == "Chance")
	{
		DrawCardMessage = CreateWidget<UBLPUWDrawCardMessage>(World, DrawChanceCardMessageClass);
	}
	else
	{
		DrawCardMessage = CreateWidget<UBLPUWDrawCardMessage>(World, DrawChestCardMessageClass);
	}

	if (!DrawCardMessage) return;

	DrawCardMessage->Setup(Heading, Description);
	DrawCardMessageSlot->AddChild(DrawCardMessage);
}

void UBLPUWGameMenu::RefreshPlayerList()
{
	PlayerCardWrapBox->ClearChildren();
	
	UWorld* World = GetWorld();
	if (!World) return;
	ABLPGameState* GameStatePtr = World->GetGameState<ABLPGameState>();
	if (!GameStatePtr) return;
	
	TArray<TObjectPtr<APlayerState>> PlayerArray = GameStatePtr->PlayerArray;
	for (int i = 0; i < PlayerArray.Num(); i++)
	{
		UBLPUWPlayerCard* PlayerCard = CreateWidget<UBLPUWPlayerCard>(World, PlayerCardClass);
		if (!PlayerCard) return;
		PlayerCard->PlayerNameText->SetText(FText::FromString(PlayerArray[i]->GetPlayerName()));
		PlayerCardWrapBox->AddChild(PlayerCard);
	}
}
