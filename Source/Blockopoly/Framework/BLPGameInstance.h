// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "BLPGameInstance.generated.h"

class UUserWidget;
class UBLPUWMainMenu;
class UBLPUWPauseMenu;
class UBLPUWLobbyMenu;
class UBLPUWGameMenu;
class UBLPUWPlayerCard;
class UBLPUWPlayerCardLobby;
class ABLPPlayerController;

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API UBLPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UBLPGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION(BlueprintCallable)
	void LoadPauseMenu();
	
	UFUNCTION()
	void QuitToMainMenu(ABLPPlayerController* BLPPlayerControllerPtr);

	UFUNCTION()
	void QuitGame();

	UFUNCTION(exec)
	void HostSession(FString ServerName);

	UFUNCTION(exec)
	void JoinSession(uint32 Index);

	// Method that searches for available sessions
	UFUNCTION()
	void RefreshSessionList();

	// Method that starts the created session so no more clients can join
	void StartSession();

private:
	// UI
	// Reference widget blueprints
	TSubclassOf<UUserWidget> MainMenuClass;
	TSubclassOf<UUserWidget> PauseMenuClass;
	TSubclassOf<UUserWidget> GameMenuClass;
	TSubclassOf<UUserWidget> LobbyMenuClass;

	// References to created user widgets
	UPROPERTY()
	UBLPUWMainMenu* MainMenu;
	UPROPERTY()
	UBLPUWPauseMenu* PauseMenu;
	UPROPERTY()
	UBLPUWGameMenu* GameMenu;
	UPROPERTY()
	UBLPUWLobbyMenu* LobbyMenu;
    	
	// Pointer to the current session interface being used
	IOnlineSessionPtr SessionInterface;

	// Method that creates a new session  
	void CreateSession();

	// Callbacks for session delegates
	// After a new session is created, server travel to LVL_Lobby and have it listen for client connections
	void OnCreateSessionComplete(FName SessionName, bool Success);
	// After current session is destroyed create a new one
	void OnDestroySessionComplete(FName SessionName, bool Success);
	// After sessions are found send session data to MainMenu for UI element creation
	void OnFindSessionsComplete(bool Success);
	// After session is joined, client travel to map session is currently on
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	// After current session fails return all PlayerControllers to main menu
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	// User entered server name
	FString DesiredSessionName;

	// Shared ptr to a session search result object (object that stores session search results)
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

};
