// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BLPUserWidget.h"
#include "BLPUWSessionEntry.generated.h"

class UTextBlock;
class UBLPUWMainMenu;
class UButton;
 
UCLASS()
class BLOCKOPOLY_API UBLPUWSessionEntry : public UBLPUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* SessionEntryBtn;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* SessionNameText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* HostUsernameText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* ConnectionFractionText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* PingText;
	
	UPROPERTY(BlueprintReadOnly)
		int Ping;
	
	// Property that keeps track if this session entry is selected, used for UI purposes (changes color if selected)
	UPROPERTY(BlueprintReadOnly)
		bool Selected = false;

	// Method that sets up this session entry
	void Setup(UBLPUWMainMenu* InParent, uint32 InIndex);

private:
	
	// Assigned index for this session entry, corresponds to the index of this session in SearchResults array of the 
	// SessionSearch object located in GameInstance
	UPROPERTY()
		uint32 Index;
	// A reference to the parent (parent in this case is the MainMenu)
	UPROPERTY()
		UBLPUWMainMenu* Parent;
	// Callback when user selects this session entry
	UFUNCTION()
		void OnClick();

};
