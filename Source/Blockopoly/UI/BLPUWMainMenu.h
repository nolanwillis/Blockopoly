// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BLPUserWidget.h"
#include "BLPUWMainMenu.generated.h"

class UBLPUWSettingsMenu;
class UButton;
class UWidgetSwitcher;
class UWidget;
class UEditableTextBox;
class UPanelWidget;

// Struct that holds data for each sesion entry
USTRUCT()
struct FSessionData
{
	GENERATED_BODY()

	FString SessionName;
	FString HostUsername;
	uint16 SessionPlayerCount;
	uint16 SessionMaxPlayerCount;
	int Ping;
};


UCLASS()
class BLOCKOPOLY_API UBLPUWMainMenu : public UBLPUserWidget
{
	GENERATED_BODY()

public:
	UBLPUWMainMenu(const FObjectInitializer& ObjectInitializer);

	 void ConstructSessionBrowser(TArray<FSessionData> SessionDataList);
	
	// Sets the index of the selected session
	void SetSelectedIndex(uint32 Index);
	
	void CloseSettingsMenu();

protected:
	virtual void NativeConstruct();
	
private:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MainWidgetSwitcher;
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* SubWidgetSwitcher;

	// Submenu references
	UPROPERTY(meta = (BindWidget))
	UWidget* Menu;
	UPROPERTY(meta = (BindWidget))
	UWidget* HostMenu;
	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;
	
	UPROPERTY(meta = (BindWidget))
	UButton* HostMenuBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* HostServerBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* HostMenuBackBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* JoinMenuBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* JoinServerBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* JoinMenuBackBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* SettingsMenuBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* QuitBtn;

	// Server browser scroll box reference
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* SessionBrowserScrollBox;

	// WBP References
	TSubclassOf<UUserWidget> SettingsMenuClass;
	TSubclassOf<UUserWidget> SessionEntryClass;

	// Index to keep track of the selected session in the server browser
	TOptional<uint32> SelectedIndex;

	// Session host name editable text box reference 
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* SessionHostName;

	// Button callbacks
	UFUNCTION()
	void OpenHostMenu();
	UFUNCTION()
	void CloseHostMenu();
	UFUNCTION()
	void HostServer();
	UFUNCTION()
	void OpenJoinMenu();
	UFUNCTION()
	void CloseJoinMenu();
	UFUNCTION()
	void JoinServer();
	UFUNCTION()
	void OpenSettingsMenu();
	UFUNCTION()
	void QuitGame();

	// Method that updates the sessions in the server browser when a session is selected
	void UpdateChildren();
};
