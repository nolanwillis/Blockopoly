// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPUWMainMenu.h"
#include "BLPUWSessionEntry.h"
#include "BLPUWSettingsMenu.h"
#include "../Framework/BLPGameInstance.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Widget.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Overlay.h"

UBLPUWMainMenu::UBLPUWMainMenu(const FObjectInitializer& ObjectInitializer)
{
	// Gets reference to WBP_SettingsMenu
	ConstructorHelpers::FClassFinder<UUserWidget> WBP_SettingsMenu(TEXT("/Game/Core/UI/WBP_SettingsMenu"));
	if (!WBP_SettingsMenu.Class) return;
	SettingsMenuClass = WBP_SettingsMenu.Class;
	
	// Gets reference to WBP_SessionEntry
	ConstructorHelpers::FClassFinder<UUserWidget> WBP_SessionEntry(TEXT("/Game/Core/UI/WBP_SessionEntry"));
	if (!WBP_SessionEntry.Class) return;
	SessionEntryClass = WBP_SessionEntry.Class;
}

void UBLPUWMainMenu::NativeConstruct()
{
	if (!HostMenuBtn) return;
	HostMenuBtn->OnClicked.AddDynamic(this, &UBLPUWMainMenu::OpenHostMenu);
	if (!HostMenuBackBtn) return;
	HostMenuBackBtn->OnClicked.AddDynamic(this, &UBLPUWMainMenu::CloseHostMenu);
	if (!HostServerBtn) return;
	HostServerBtn->OnClicked.AddDynamic(this, &UBLPUWMainMenu::HostServer);
	if (!JoinMenuBtn) return;
	JoinMenuBtn->OnClicked.AddDynamic(this, &UBLPUWMainMenu::OpenJoinMenu);
	if (!JoinMenuBackBtn) return;
	JoinMenuBackBtn->OnClicked.AddDynamic(this, &UBLPUWMainMenu::CloseJoinMenu);
	if (!JoinServerBtn) return;
	JoinServerBtn->OnClicked.AddDynamic(this, &UBLPUWMainMenu::JoinServer);
	if (!SettingsMenuBtn) return;
	SettingsMenuBtn->OnClicked.AddDynamic(this, &UBLPUWMainMenu::OpenSettingsMenu);
	if (!QuitBtn) return;
	QuitBtn->OnClicked.AddDynamic(this, &UBLPUWMainMenu::QuitGame);

	UWorld* World = GetWorld();
	if (!World) return;
	UBLPUWSettingsMenu* SettingsMenu = CreateWidget<UBLPUWSettingsMenu>(World, SettingsMenuClass);
	if (!SettingsMenu) return;
	SettingsMenu->SetParent(this);
	if (!MainWidgetSwitcher) return;
	MainWidgetSwitcher->AddChild(SettingsMenu);
	
}

// Sets the index of the selected SessionEntry and updates the UI to reflect this change
void UBLPUWMainMenu::SetSelectedIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UBLPUWMainMenu::OpenHostMenu()
{
	if (!SubWidgetSwitcher) return;
	if (!HostMenu) return;
	SubWidgetSwitcher->SetActiveWidget(HostMenu);
}

void UBLPUWMainMenu::CloseHostMenu()
{
	if (!SubWidgetSwitcher) return;
	if (!HostMenu) return;
	SubWidgetSwitcher->SetActiveWidget(Menu);
}

void UBLPUWMainMenu::HostServer()
{
	UBLPGameInstance* GameInstance = Cast<UBLPGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		FString SessionName = SessionHostName->Text.ToString();
		GameInstance->HostSession(SessionName);
	}
}

void UBLPUWMainMenu::OpenJoinMenu()
{
	if (!SubWidgetSwitcher) return;
	if (!JoinMenu) return;
	SubWidgetSwitcher->SetActiveWidget(JoinMenu);
	UBLPGameInstance* GameInstance = Cast<UBLPGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->RefreshSessionList();
	}
}

void UBLPUWMainMenu::CloseJoinMenu()
{
	if (!SubWidgetSwitcher) return;
	if (!JoinMenu) return;
	SubWidgetSwitcher->SetActiveWidget(Menu);
}

void UBLPUWMainMenu::JoinServer()
{
	UBLPGameInstance* GameInstance = Cast<UBLPGameInstance>(GetGameInstance());
	if (SelectedIndex.IsSet() && GameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index: %d"), SelectedIndex.GetValue());
		GameInstance->JoinSession(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index not set"));
	}
}

void UBLPUWMainMenu::OpenSettingsMenu()
{
	if (!MainWidgetSwitcher) return;
	MainWidgetSwitcher->SetActiveWidgetIndex(1);
}

void UBLPUWMainMenu::CloseSettingsMenu()
{
	if (!MainWidgetSwitcher) return;
	MainWidgetSwitcher->SetActiveWidgetIndex(0);
}

void UBLPUWMainMenu::QuitGame()
{
	UBLPGameInstance* GameInstance = Cast<UBLPGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->QuitGame();
	}
	UE_LOG(LogTemp, Warning, TEXT("Quit game function called in main menu cpp"))
}

// Method that builds the session browser
void UBLPUWMainMenu::ConstructSessionBrowser(TArray<FSessionData> SessionDataList)
{
	UWorld* World = this->GetWorld();
	if (!World) return;
	// Clear existing children from the SessionBrowserScrollBox
	SessionBrowserScrollBox->ClearChildren();
	// Index value to increment, used to setup each SessionEntry
	uint32 i = 0;
	for (const FSessionData& SessionData : SessionDataList)
	{
		UBLPUWSessionEntry* SessionEntry = CreateWidget<UBLPUWSessionEntry>(World, SessionEntryClass);
		if (!SessionEntry) return;
		// Set UI/Data of spawned SessionEntry to reflect it's corresponding FSessionData struct information
		SessionEntry->SessionNameText->SetText(FText::FromString(SessionData.SessionName));
		SessionEntry->HostUsernameText->SetText(FText::FromString(SessionData.HostUsername));
		FString FractionString = FString::Printf(TEXT("%d/%d"), SessionData.SessionPlayerCount + 1, SessionData.SessionMaxPlayerCount);
		SessionEntry->ConnectionFractionText->SetText(FText::FromString(FractionString));
		SessionEntry->Ping = SessionData.Ping;
		FString PingString = FString::Printf(TEXT("%dms"), SessionData.Ping);
		SessionEntry->PingText->SetText(FText::FromString(PingString));
		// Call setup method to assign this SessionEntry's parent and index
		SessionEntry->Setup(this, i);
		// Increment the Index value
		++i;
		// Add SessionEntry UI element to the SessionBrowserScrollBox
		SessionBrowserScrollBox->AddChild(SessionEntry);
	}
}

// Method that updates the children of the SessionEntryBrowserScrollBox to reflect a 
// session selection from the user
void UBLPUWMainMenu::UpdateChildren()
{
	UE_LOG(LogTemp, Warning, TEXT("Update children method called"));
	for (int32 i = 0; i < SessionBrowserScrollBox->GetChildrenCount(); ++i)
	{
		UBLPUWSessionEntry* Entry = Cast<UBLPUWSessionEntry>(SessionBrowserScrollBox->GetChildAt(i));
		if (Entry)
		{
			Entry->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}
