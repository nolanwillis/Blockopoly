// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPUWMainMenu.h"
#include "BLPUWSessionEntry.h"
#include "../Framework/BLPGameInstance.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Widget.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"

UBLPUWMainMenu::UBLPUWMainMenu(const FObjectInitializer& ObjectInitializer)
{
	// Gets reference to WBP_SessionEntry
	ConstructorHelpers::FClassFinder<UUserWidget> WBP_SessionEntry(TEXT("/Game/Core/UI/WBP_SessionEntry"));
	if (!WBP_SessionEntry.Class) return;
	SessionEntryClass = WBP_SessionEntry.Class;
}

bool UBLPUWMainMenu::Initialize()
{
	// Call parent version of function and store result in variable
	bool Success = Super::Initialize();
	if (!Success) return false;

	// Bind host/join server functions to OnClicked delegate of host/join button
	if (!HostMenuBtn) return false;
	HostMenuBtn->OnClicked.AddDynamic(this, &UBLPUWMainMenu::OpenHostMenu);
	if (!HostMenuBackBtn) return false;
	HostMenuBackBtn->OnClicked.AddDynamic(this, &UBLPUWMainMenu::CloseHostMenu);
	if (!HostServerBtn) return false;
	HostServerBtn->OnClicked.AddDynamic(this, &UBLPUWMainMenu::HostServer);
	if (!JoinMenuBtn) return false;
	JoinMenuBtn->OnClicked.AddDynamic(this, &UBLPUWMainMenu::OpenJoinMenu);
	if (!JoinMenuBackBtn) return false;
	JoinMenuBackBtn->OnClicked.AddDynamic(this, &UBLPUWMainMenu::CloseJoinMenu);
	if (!JoinServerBtn) return false;
	JoinServerBtn->OnClicked.AddDynamic(this, &UBLPUWMainMenu::JoinServer);
	if (!QuitBtn) return false;
	QuitBtn->OnClicked.AddDynamic(this, &UBLPUWMainMenu::QuitGame);
	return true;

}

// Sets the index of the selected SessionEntry and updates the UI to reflect this change
void UBLPUWMainMenu::SetSelectedIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UBLPUWMainMenu::OpenHostMenu()
{
	if (!MenuSwitcher) return;
	if (!HostMenu) return;
	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UBLPUWMainMenu::CloseHostMenu()
{
	if (!MenuSwitcher) return;
	if (!HostMenu) return;
	MenuSwitcher->SetActiveWidget(Menu);
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
	if (!MenuSwitcher) return;
	if (!JoinMenu) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
	UBLPGameInstance* GameInstance = Cast<UBLPGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->RefreshSessionList();
	}
}

void UBLPUWMainMenu::CloseJoinMenu()
{
	if (!MenuSwitcher) return;
	if (!JoinMenu) return;
	MenuSwitcher->SetActiveWidget(Menu);
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
