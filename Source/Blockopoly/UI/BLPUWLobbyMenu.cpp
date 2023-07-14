// Fill out your copyright notice in the Description page of Project Settings.


#include "./BLPUWLobbyMenu.h"
#include "../Framework/State/BLPPlayerState.h"
#include "../Framework/State/BLPGameState.h"
#include "../Framework/Controllers/BLPPlayerController.h"
#include "./BLPUWLobbyPlayerCard.h"

#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"


UBLPUWLobbyMenu::UBLPUWLobbyMenu()
{
	// Gets reference to WBP_LobbyPlayerCard
	const ConstructorHelpers::FClassFinder<UUserWidget> WBP_LobbyPlayerCard(TEXT("/Game/Core/UI/WBP_LobbyPlayerCard"));
	if (!WBP_LobbyPlayerCard.Class) return;
	LobbyPlayerCardClass = WBP_LobbyPlayerCard.Class;
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

	UWorld* World = GetWorld();
	if (!World) return;
	const ABLPGameState* BLPGameStatePtr = World->GetGameState<ABLPGameState>();
	if (!BLPGameStatePtr) return;

	// Required for initial setup
	Refresh();
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
	if (!BLPGameStatePtr) return;

	TArray<TObjectPtr<APlayerState>> PlayerArray = BLPGameStatePtr->PlayerArray;
	TArray<int> ReadyStatusArray = BLPGameStatePtr->GetReadyStatusArray();
    	
	for (int i = 0; i < PlayerArray.Num(); i++)
	{
		ABLPPlayerState* BLPPlayerStatePtr = Cast<ABLPPlayerState>(PlayerArray[i]);
		if (!BLPGameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWLobbyMenu: BLPPlayerStatePtr is null")); return; }
    
		UBLPUWLobbyPlayerCard* LobbyPlayerCard = CreateWidget<UBLPUWLobbyPlayerCard>(World, LobbyPlayerCardClass);
		if (!LobbyPlayerCard) return;
    
		if (ReadyStatusArray[BLPPlayerStatePtr->GetBLPPlayerId()]) LobbyPlayerCard->CheckImage->SetVisibility(ESlateVisibility::Visible);
		else LobbyPlayerCard->CheckImage->SetVisibility(ESlateVisibility::Hidden);
    		
		LobbyPlayerCard->PlayerNameText->SetText(FText::FromString(BLPPlayerStatePtr->GetPlayerName()));
		PlayerCardWrapBox->AddChild(LobbyPlayerCard);
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






