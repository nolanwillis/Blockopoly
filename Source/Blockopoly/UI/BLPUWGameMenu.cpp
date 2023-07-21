// Fill out your copyright notice in the Description page of Project Settings.

#include "BLPUWGameMenu.h"

#include "BLPUWNotification.h"
#include "BLPUWPropertyMenu.h"
#include "BLPUWPlayerCard.h"
#include "../Framework/Pawns/BLPAvatar.h"
#include "../Framework/Controllers/BLPPlayerController.h"
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

	const ConstructorHelpers::FClassFinder<UUserWidget> WBP_ChanceCardNotification(TEXT("/Game/Core/UI/WBP_ChanceCardNotification"));
	if (!WBP_ChanceCardNotification.Class) return;
	ChanceCardNotificationClass = WBP_ChanceCardNotification.Class;

	const ConstructorHelpers::FClassFinder<UUserWidget> WBP_ChestCardNotification(TEXT("/Game/Core/UI/WBP_ChestCardNotification"));
	if (!WBP_ChestCardNotification.Class) return;
	ChestCardNotificationClass = WBP_ChestCardNotification.Class;

	const ConstructorHelpers::FClassFinder<UUserWidget> WBP_RollNotification(TEXT("/Game/Core/UI/WBP_RollNotification"));
	if (!WBP_RollNotification.Class) return;
	RollNotificationClass = WBP_RollNotification.Class;

	const ConstructorHelpers::FClassFinder<UUserWidget> WBP_ForfeitNotification(TEXT("/Game/Core/UI/WBP_ForfeitNotification"));
	if (!WBP_ForfeitNotification.Class) return;
	ForfeitNotificationClass = WBP_ForfeitNotification.Class;
	
	const ConstructorHelpers::FClassFinder<UUserWidget> WBP_LeaveNotification(TEXT("/Game/Core/UI/WBP_LeaveNotification"));
	if (!WBP_LeaveNotification.Class) return;
	LeaveNotificationClass = WBP_LeaveNotification.Class;
}

 void UBLPUWGameMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind button callbacks to OnClicked delegate
	if (!PropertyMenuBtn) return;
	PropertyMenuBtn->OnClicked.AddDynamic(this, &UBLPUWGameMenu::PropertyMenuBtnClicked);
	if (!BuyBtn) return;
	BuyBtn->OnClicked.AddDynamic(this, &UBLPUWGameMenu::BuyBtnClicked);
	if (!RollBtn) return;
	RollBtn->OnClicked.AddDynamic(this, &UBLPUWGameMenu::RollBtnClicked);
	if (!FinishTurnBtn) return;
	FinishTurnBtn->OnClicked.AddDynamic(this, &UBLPUWGameMenu::FinishTurnBtnClicked);
	if (!SkipJailBtn) return;
	SkipJailBtn->OnClicked.AddDynamic(this, &UBLPUWGameMenu::SkipJailBtnClicked);
	if (!ForfeitBtn) return;
	ForfeitBtn->OnClicked.AddDynamic(this, &UBLPUWGameMenu::ForfeitBtnClicked);
    	
	ABLPPlayerController* BLPPlayerControllerPtr = GetOwningPlayer<ABLPPlayerController>();
	if (!BLPPlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: BLPPlayerControllerPtr is null")); return; }
	
	ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
	if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: BLPPlayerStatePtr is null")); return; }
	
	BLPPlayerStatePtr->PlayerUpIdDelegate.AddUObject(this, &UBLPUWGameMenu::PlayerUpId);
	BLPPlayerStatePtr->RefreshUIDelegate.AddUObject(this, &UBLPUWGameMenu::RefreshPlayerList);
	BLPPlayerStatePtr->OnBalanceChangedDelegate.AddUObject(this, &UBLPUWGameMenu::UpdateBalance);
	BLPPlayerStatePtr->InJailDelegate.BindUObject(this, &UBLPUWGameMenu::InJail);
	BLPPlayerStatePtr->OutOfJailDelegate.BindUObject(this, &UBLPUWGameMenu::OutOfJail);
	BLPPlayerStatePtr->JailSkipDelegate.BindUObject(this, &UBLPUWGameMenu::UpdateJailSkipCounter);
	BLPPlayerStatePtr->CanBuyDelegate.BindUObject(this, &UBLPUWGameMenu::CanBuy);
	BLPPlayerStatePtr->HasRolledDelegate.BindUObject(this, &UBLPUWGameMenu::HasRolled);
	BLPPlayerStatePtr->NotificationDelegate.BindUObject(this, &UBLPUWGameMenu::AddNotification);
	
	// Set initial turn status for the owner of this menu.
	BLPPlayerControllerPtr->Server_SetInitialTurnStatus(BLPPlayerStatePtr);
}

void UBLPUWGameMenu::PropertyMenuBtnClicked()
{
	MainWidgetSwitcher->SetActiveWidgetIndex(1);
	PropertyMenu->RefreshPropertyWrapBox();
	
	UE_LOG(LogTemp, Warning, TEXT("Property menu button clicked"));
}

void UBLPUWGameMenu::BuyBtnClicked()
{
	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(GetOwningPlayer());
	if (!PlayerControllerPtr) return;

	const UWorld* World = GetWorld();
	if (!World) return;
	ABLPGameState* BLPGameStatePtr = World->GetGameState<ABLPGameState>(); 
	ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
	
	PlayerControllerPtr->Server_BuyPropertySpace(BLPPlayerStatePtr, BLPGameStatePtr);

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
	ABLPPlayerState* PlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
	ABLPAvatar* AvatarPtr = GetOwningPlayerPawn<ABLPAvatar>();
	
	PlayerControllerPtr->Server_Roll(AvatarPtr, PlayerStatePtr, GameStatePtr);

	RollBtn->SetVisibility(ESlateVisibility::Hidden);
	
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

void UBLPUWGameMenu::SkipJailBtnClicked()
{
	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(GetOwningPlayer());
 	if (!PlayerControllerPtr) return;
 	
 	const UWorld* World = GetWorld();
 	if (!World) return;
 	ABLPGameState* GameStatePtr = World->GetGameState<ABLPGameState>(); 
 	ABLPPlayerState* PlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
 	
 	PlayerControllerPtr->Server_SkipJail(PlayerStatePtr);

	SkipJailBtn->SetVisibility(ESlateVisibility::Hidden);
	LargeNotificationWidgetSwitcher->SetActiveWidgetIndex(0);
 	UE_LOG(LogTemp, Warning, TEXT("Finish turn button clicked"));
}

void UBLPUWGameMenu::ForfeitBtnClicked()
{
	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(GetOwningPlayer());
	if (!PlayerControllerPtr) return;
    
	const UWorld* World = GetWorld();
	if (!World) return;
	ABLPGameState* BLPGameStatePtr = World->GetGameState<ABLPGameState>(); 
	ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
    	
	PlayerControllerPtr->Server_Forfeit(BLPPlayerStatePtr, BLPGameStatePtr);

	BankruptText->SetText(FText::FromString("You have forfeit!"));

	ForfeitBtn->SetVisibility(ESlateVisibility::Hidden);
	
	UE_LOG(LogTemp, Warning, TEXT("Forfeit button clicked"));
}

void UBLPUWGameMenu::PlayerUpId()
{
	const UWorld* World = GetWorld();
	if (!World) return;
	const ABLPGameState* BLPGameStatePtr = World->GetGameState<ABLPGameState>();
	const ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
	if (!BLPGameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: BLPGameStatePtr is null")); return; }
	if (!BLPPlayerStatePtr) UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: PlayerStatePtr is null"));
    	
	// If its my turn
	if (BLPPlayerStatePtr->GetBLPPlayerId() == BLPPlayerStatePtr->GetPlayerUpId())
	{
		FinishTurnBtn->SetVisibility(ESlateVisibility::Hidden);
		YourTurnText->SetText(FText::FromString("It's your turn"));

		if (BLPPlayerStatePtr->GetJailCounter() == 0)
		{
			// Only let RollBtn be visible if not in jail.
			RollBtn->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			// If in jail, pass true in to HasRolled() so FinishTurnBtn becomes visible.
			HasRolled(true);
			RollBtn->SetVisibility(ESlateVisibility::Hidden);
			if (BLPPlayerStatePtr->GetJailSkipCounter() >= 1) SkipJailBtn->SetVisibility(ESlateVisibility::Visible);

		}		
	}
	// If its not my turn
	else
	{
    	const ABLPPlayerState* PlayerUpBLPPlayerState = BLPGameStatePtr->GetBLPPlayerStateFromId(BLPPlayerStatePtr->GetPlayerUpId());
		if (!PlayerUpBLPPlayerState) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: PlayerUpBLPPlayerState is null")); return; }

		const FString PlayerUpName = PlayerUpBLPPlayerState->GetPlayerName();
    	
    	YourTurnText->SetText(FText::FromString(PlayerUpName + " is up"));
    	RollBtn->SetVisibility(ESlateVisibility::Hidden);
    	FinishTurnBtn->SetVisibility(ESlateVisibility::Hidden);
    	BuyBtn->SetVisibility(ESlateVisibility::Hidden);
    	SkipJailBtn->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBLPUWGameMenu::UpdateBalance(const int NewBalance)
{
	BalanceText->SetText(FText::AsNumber(NewBalance));
	CheckBankruptcyStatus();
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
	LargeNotificationWidgetSwitcher->SetActiveWidgetIndex(1);
	RollBtn->SetVisibility(ESlateVisibility::Hidden);
}

void UBLPUWGameMenu::OutOfJail()
{
	LargeNotificationWidgetSwitcher->SetActiveWidgetIndex(0);
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
	const ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
	if (!BLPPlayerStatePtr) UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: PlayerStatePtr is null"));

	// Check if bankrupt to ensure FinishTurnBtn does not become visible
	if (BLPPlayerStatePtr->GetBalance() < 0)
	{
		FinishTurnBtn->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

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

void UBLPUWGameMenu::UpdateJailSkipCounter(const int& Value)
{
	if (Value == 0)
	{
		JailSkipCounterTextBlock->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		JailSkipCounterTextBlock->SetVisibility(ESlateVisibility::Visible);
		JailSkipCounterTextBlock->SetText(FText::FromString("X " + FString::FromInt(Value)));
		JailSkipCounterContainer->SetVisibility(ESlateVisibility::HitTestInvisible);
		JailSkipCounterImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UBLPUWGameMenu::CheckBankruptcyStatus() const
{
	const ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
	if (!BLPPlayerStatePtr) UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: PlayerStatePtr is null"));

	if (BLPPlayerStatePtr->GetBalance() < 0)
	{
		LargeNotificationWidgetSwitcher->SetActiveWidgetIndex(2);
		FinishTurnBtn->SetVisibility(ESlateVisibility::Hidden);
		ForfeitBtn->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		LargeNotificationWidgetSwitcher->SetActiveWidgetIndex(0);
		FinishTurnBtn->SetVisibility(ESlateVisibility::Visible);
		ForfeitBtn->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBLPUWGameMenu::AddNotification(const FString& Type, const FString& Heading, const FString& Description)
{
	UWorld* World = GetWorld();
	if (!World) return;

	UBLPUWNotification* Notification = nullptr;
	
	if (Type == "Chance")
	{
		CardNotificationSlot->ClearChildren();
		Notification = CreateWidget<UBLPUWNotification>(World, ChanceCardNotificationClass);
		if (!Notification) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: Notification is null")); return; }
		Notification->Setup(Type, Heading, Description, this);
		CardNotificationSlot->AddChild(Notification);
	}
	else if (Type == "Community Chest")
	{
		CardNotificationSlot->ClearChildren();
		Notification = CreateWidget<UBLPUWNotification>(World, ChestCardNotificationClass);
		if (!Notification) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: Notification is null")); return; }
		Notification->Setup(Type, Heading, Description, this);
		CardNotificationSlot->AddChild(Notification);
        
	}
	else if (Type == "Roll")
	{
		BannerNotificationSlot->ClearChildren();
		Notification = CreateWidget<UBLPUWNotification>(World, RollNotificationClass);
		if (!Notification) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: Notification is null")); return; }
		Notification->Setup(Type, Heading, Description, this);
		BannerNotificationSlot->AddChild(Notification);
	}
	else if (Type == "Forfeit")
	{
		BannerNotificationSlot->ClearChildren();
		Notification = CreateWidget<UBLPUWNotification>(World, ForfeitNotificationClass);
		if (!Notification) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: Notification is null")); return; }
		Notification->Setup(Type, Heading, Description, this);
		BannerNotificationSlot->AddChild(Notification);
	}
	else if (Type == "Leave")
	{
		BannerNotificationSlot->ClearChildren();
		Notification = CreateWidget<UBLPUWNotification>(World, LeaveNotificationClass);
		if (!Notification) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: Notification is null")); return; }
		Notification->Setup(Type, Heading, Description, this);
		BannerNotificationSlot->AddChild(Notification);
	}
}

void UBLPUWGameMenu::RefreshPlayerList()
{
	PlayerCardWrapBox->ClearChildren();
	
	UWorld* World = GetWorld();
	if (!World) return;
	const ABLPGameState* BLPGameStatePtr = World->GetGameState<ABLPGameState>();
	const ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
	if (!BLPGameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: BLPGameStatePtr is null ")); return;}
	if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: BLPPlayerStatePtr is null ")); return;}

	
	const TArray<TObjectPtr<APlayerState>> PlayerArray = BLPGameStatePtr->PlayerArray;
	
	for (APlayerState* PlayerState : PlayerArray)
	{
		const ABLPPlayerState* BLPPlayerState = Cast<ABLPPlayerState>(PlayerState);
		if (!BLPPlayerState) { UE_LOG(LogTemp, Warning, TEXT("BLPUWGameMenu: BLPPlayerStatePtr is null ")); return;}

		if (BLPPlayerState->GetIsLeaving()) continue;
		
		UBLPUWPlayerCard* PlayerCard = CreateWidget<UBLPUWPlayerCard>(World, PlayerCardClass);
		if (!PlayerCard) return;
		PlayerCard->PlayerNameText->SetText(FText::FromString(BLPPlayerState->GetPlayerName()));

		TArray<int> ForfeitedPlayersArray = BLPGameStatePtr->GetForfeitedPlayersArray();

		if (ForfeitedPlayersArray.Contains(BLPPlayerState->GetBLPPlayerId()))
		{
			PlayerCard->SetRenderOpacity(0.2f);
		}
		
		PlayerCardWrapBox->AddChild(PlayerCard);
	}
}
