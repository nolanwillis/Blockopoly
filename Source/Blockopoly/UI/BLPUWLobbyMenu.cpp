// Fill out your copyright notice in the Description page of Project Settings.


#include "./BLPUWLobbyMenu.h"
#include "../Framework/State/BLPPlayerState.h"
#include "../Framework/State/BLPGameState.h"
#include "../Framework/Controllers/BLPPlayerController.h"
#include "./BLPUWPlayerCard.h"

#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"


UBLPUWLobbyMenu::UBLPUWLobbyMenu()
{
	// Gets reference to WBP_LobbyPlayerCard
	const ConstructorHelpers::FClassFinder<UUserWidget> WBP_PlayerCard(TEXT("/Game/Core/UI/WBP_PlayerCard"));
	if (!WBP_PlayerCard.Class) return;
	PlayerCardClass = WBP_PlayerCard.Class;
}

void UBLPUWLobbyMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ReadyBtn) return;
	ReadyBtn->OnClicked.AddDynamic(this, &UBLPUWLobbyMenu::ReadyBtnClicked);
	if (!PlayBtn) return;
	PlayBtn->OnClicked.AddDynamic(this, &UBLPUWLobbyMenu::PlayBtnClicked);
	
	ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
	if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWLobbyMenu: BLPPlayerStatePtr is null")); return; }

	BLPPlayerStatePtr->RefreshUIDelegate.AddUObject(this, &UBLPUWLobbyMenu::Refresh);
}

void UBLPUWLobbyMenu::ReadyBtnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("BLPUWLobbyMenu: ReadyBtn Clicked!"));

	UWorld* World = GetWorld();
	if (!World) return;

	ABLPGameState* BLPGameStatePtr = Cast<ABLPGameState>(World->GetGameState());
	ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
	ABLPPlayerController* BLPPlayerControllerPtr = GetOwningPlayer<ABLPPlayerController>();

	if (!BLPGameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWLobbyMenu: BLPGameStatePtr is null")); return; }
	if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWLobbyMenu: BLPPlayerStatePtr is null")); return; }
	if (!BLPPlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWLobbyMenu: BLPPlayerControllerPtr is null")); return; }

	if (BLPGameStatePtr->GetReadyStatusArray()[BLPPlayerStatePtr->GetBLPPlayerId()]) ReadyBtnTextBlock->SetText(FText::FromString("Ready Up"));
	else ReadyBtnTextBlock->SetText(FText::FromString("Not Ready"));

	BLPPlayerControllerPtr->Server_ToggleIsReady(BLPPlayerStatePtr, BLPGameStatePtr);
}

void UBLPUWLobbyMenu::PlayBtnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("BLPUWLobbyMenu: PlayBtn Clicked!"));
 
 	UWorld* World = GetWorld();
 	if (!World) return;
 
 	ABLPGameState* BLPGameStatePtr = Cast<ABLPGameState>(World->GetGameState());
 	ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
 	ABLPPlayerController* BLPPlayerControllerPtr = GetOwningPlayer<ABLPPlayerController>();
 
 	if (!BLPGameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWLobbyMenu: BLPGameStatePtr is null")); return; }
 	if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWLobbyMenu: BLPPlayerStatePtr is null")); return; }
 	if (!BLPPlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWLobbyMenu: BLPPlayerControllerPtr is null")); return; }
 
 	if (BLPGameStatePtr->GetReadyStatusArray()[BLPPlayerStatePtr->GetBLPPlayerId()]) ReadyBtnTextBlock->SetText(FText::FromString("Ready Up"));
 	else ReadyBtnTextBlock->SetText(FText::FromString("Not Ready"));
 
 	BLPPlayerControllerPtr->Server_PlayGame(BLPPlayerStatePtr, BLPGameStatePtr);

	if (BLPPlayerStatePtr->GetLocalRole() == ROLE_Authority) UE_LOG(LogTemp, Warning, TEXT("Authority"));
}

void UBLPUWLobbyMenu::Refresh()
{
	RefreshPlayerList();
	CheckAllReadyStatus();
}

void UBLPUWLobbyMenu::RefreshPlayerList()
{
	PlayerCardWrapBox->ClearChildren();
        	
	UWorld* World = GetWorld();
	if (!World) return;
	const ABLPGameState* BLPGameStatePtr = World->GetGameState<ABLPGameState>();
	if (!BLPGameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWLobbyMenu: BLPGameStatePtr is null"));  return; };
	
	const TArray<TObjectPtr<APlayerState>> PlayerArray = BLPGameStatePtr->PlayerArray;
	TArray<int> ReadyStatusArray = BLPGameStatePtr->GetReadyStatusArray();
    	
	for (APlayerState* PlayerStatePtr : PlayerArray)
	{
		if (!PlayerStatePtr) UE_LOG(LogTemp, Warning, TEXT("BLPUWLobbyMenu: PlayerStatePtr is null"));
		const ABLPPlayerState* BLPPlayerStatePtr = Cast<ABLPPlayerState>(PlayerStatePtr);
		if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWLobbyMenu 111: BLPPlayerStatePtr is null")); return; }
    
		UBLPUWPlayerCard* PlayerCard = CreateWidget<UBLPUWPlayerCard>(World, PlayerCardClass);
		if (!PlayerCard) return;

		switch(BLPPlayerStatePtr->GetBLPPlayerId())
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
		
		PlayerCard->PlayerNameText->SetText(FText::FromString(BLPPlayerStatePtr->GetPlayerName()));
		PlayerCardWrapBox->AddChild(PlayerCard);
	}
    
	UE_LOG(LogTemp, Warning, TEXT("BLPUWLobbyMenu: PlayerList Updated!"));
}

void UBLPUWLobbyMenu::CheckAllReadyStatus()
{
	UWorld* World = GetWorld();
	if (!World) return;
	const ABLPGameState* BLPGameStatePtr = World->GetGameState<ABLPGameState>();

	TArray<TObjectPtr<APlayerState>> PlayerArray = BLPGameStatePtr->PlayerArray;
	TArray<int> ReadyStatusArray = BLPGameStatePtr->GetReadyStatusArray();

	int Count = 0;
	for (const int& i : ReadyStatusArray) if(i) Count++;
	
	if (Count == BLPGameStatePtr->PlayerArray.Num())
	{
		WidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
		
		if (GetOwningPlayer()->GetLocalRole() == ROLE_Authority) WidgetSwitcher->SetActiveWidgetIndex(0);
		else WidgetSwitcher->SetActiveWidgetIndex(1);
	}
	else WidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
}






