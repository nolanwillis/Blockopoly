// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPGameInstance.h"
#include "../UI/BLPUserWidget.h"
#include "../UI/BLPUWMainMenu.h"
#include "../UI/BLPUWPauseMenu.h"
#include "../Framework/GameModes/BLPGMClassic.h"
#include "../Framework/State/BLPGameState.h"

#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"

// Constants 
const static FName SESSION_NAME = TEXT("Game");
// Name of the settings key for changing the session name
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UBLPGameInstance::UBLPGameInstance(const FObjectInitializer& ObjectInitializer)
{
	// Gets reference to WBP_MainMenu
	ConstructorHelpers::FClassFinder<UUserWidget> WBP_MainMenu(TEXT("/Game/Core/UI/WBP_MainMenu"));
	if (!WBP_MainMenu.Class) return;
	MainMenuClass = WBP_MainMenu.Class;

	// Gets reference to WBP_PauseMenu
	ConstructorHelpers::FClassFinder<UUserWidget> WBP_PauseMenu(TEXT("/Game/Core/UI/WBP_PauseMenu"));
	if (!WBP_PauseMenu.Class) return;
	PauseMenuClass = WBP_PauseMenu.Class;
}

void UBLPGameInstance::Init()
{
	Super::Init();
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
		// The session interface is not a UObject so it uses a TSharedPtr for memory
		// management
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			// Bind function to on create/destroy session delegates
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UBLPGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UBLPGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UBLPGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UBLPGameInstance::OnJoinSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found no subsystem"));
	}

	if (GEngine)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UBLPGameInstance::OnNetworkFailure);
	}
}

void UBLPGameInstance::LoadMainMenu()
{
	// Creates a WBP_MainMenu and adds it to the viewport
	if (!MainMenuClass) return;
	MainMenu = CreateWidget<UBLPUWMainMenu>(this, MainMenuClass);
	if (!MainMenu) return;
	MainMenu->Setup();
	UE_LOG(LogTemp, Warning, TEXT("LoadMainMenu called in GI"));
}

void UBLPGameInstance::LoadPauseMenu()
{
	// Creates a WBP_Pause and adds it to the viewport
	if (!PauseMenuClass) return;
	PauseMenu = CreateWidget<UBLPUWPauseMenu>(this, PauseMenuClass);
	if (!PauseMenu) return;
	PauseMenu->Setup();
}

void UBLPGameInstance::QuitToMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController) return;
	// Tells current player controller to travel to a different map
	PlayerController->ClientTravel("/Game/Maps/LVL_MainMenu", ETravelType::TRAVEL_Absolute);
}

void UBLPGameInstance::QuitGame()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController) return;
	PlayerController->ConsoleCommand(TEXT("Quit"));
}

void UBLPGameInstance::HostSession(FString ServerName)
{
	if (SessionInterface.IsValid())
	{
		// Set the desired session name that will be used in listing
		DesiredSessionName = ServerName;
		
		// If a session for this GI already exists call DestroySession, otherwise
		// create a new sesion
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (!ExistingSession)
		{
			CreateSession();
		}
		else
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
	}
}

void UBLPGameInstance::JoinSession(uint32 Index)
{
	// nullptr checks
	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	// If the MainMenu has been contructed remove it
	if (MainMenu)
	{
		MainMenu->Remove();
	}

	// Join session at given index in the SessionSearch object
	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UBLPGameInstance::RefreshSessionList()
{ 
	// Create session search result object and store found sessions in it
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Searching for Sessions..."));
		// FindSessions needs a shared reference not a shared ptr
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UBLPGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		// Create new session settings struct for a new session
		FOnlineSessionSettings SessionSettings;
		// Set bIsLANMatch to true is using NULL OSS or false if using STEAM OSS
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}
		// Set settings properties for new session
		SessionSettings.NumPublicConnections = 4;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredSessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		// Create a new session using the session settings struct
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UBLPGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

void UBLPGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	// If the session is not successfully created print warning message
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session failed to be created"))
		return;
	}
	// If the MainMenu has been constructed remove it
	if (MainMenu)
	{
		MainMenu->Remove();
	}
	// Add debug message to screen if session is created
	UEngine* Engine = GetEngine();
	if (!Engine) return;
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));
	// Server travel to LVL_Lobby
	UWorld* World = GetWorld();
	if (!World) return;
	// Move all player controllers to LVL_Lobby and ensure server is listening
	World->ServerTravel("/Game/Maps/LVL_Lobby?listen");
}

void UBLPGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	// If old session is successfully deleted, create a new session
	if (Success)
	{
		CreateSession();
	}
}

void UBLPGameInstance::OnFindSessionsComplete(bool Success)
{
	// If all these ptrs are not null
	if (Success && SessionSearch.IsValid() && MainMenu)
	{
		// Print warning when sesion search is finished
		UE_LOG(LogTemp, Warning, TEXT("Session Search Complete"))
		// Create a local FSessionData array to keep track of data associated with each session
		TArray<FSessionData> SessionList;
		// For each discovered session
		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			// Print warning for each session found
			UE_LOG(LogTemp, Warning, TEXT("Found session: %s"), *(SearchResult.GetSessionIdStr()));

			// Create FSessionData struct for this session and fill it with in
			FSessionData Data;
			
			// Out parameter for SessionSettings.Get()
			FString SessionName;
			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, SessionName))
			{
				Data.SessionName = SessionName;
			}
			else
			{
				Data.SessionName = "Could not find name";
			}
			
			Data.HostUsername = SearchResult.Session.OwningUserName;
			Data.SessionMaxPlayerCount = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.SessionPlayerCount = SearchResult.Session.NumOpenPublicConnections - Data.SessionMaxPlayerCount;
			Data.Ping = SearchResult.PingInMs;
			// Check if 
			SessionList.Add(Data);
		}
		// Give MainMenu the FSessionData from the search to populate SessionBrowserScrollBox
		MainMenu->ConstructSessionBrowser(SessionList);
	}
}

void UBLPGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	// Session is joined client travel to the address of the session
	if (!SessionInterface.IsValid()) return;
	// Out parameter for GetResolvedConnectString method
	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string"));
		return;
	}
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController) return;
	// Tells client PlayerController to travel to the map of the session 
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UBLPGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LoadMainMenu();
}


