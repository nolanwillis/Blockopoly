// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./BLPUserWidget.h"
#include "Components/Button.h"
#include "BLPUWPropertyMenu.generated.h"

class UTextBlock;
class UBLPUWPropertyTitle;
class ABLPPlayerState;
class ABLPGameState;
class ABLPPlayerController;
class ABLPPropertySpace;
class UButton;
class UWrapBox;
class UBLPUWPropertyDetails;
class UBLPUWGameMenu;

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API UBLPUWPropertyMenu : public UBLPUserWidget
{
	GENERATED_BODY()

public:
	UBLPUWPropertyMenu();

	UFUNCTION()
	void RefreshPropertyWrapBox();

	UFUNCTION()
	void RefreshPropertyDetails() const;

	void SetSelectedPropertySpace(ABLPPropertySpace* Value) { SelectedPropertySpace = Value; }
	
	void SetSelectedPropertyTitle(UBLPUWPropertyTitle* Value) { SelectedPropertyTitle = Value; }

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	ABLPPropertySpace* SelectedPropertySpace;

	UPROPERTY()
	UBLPUWPropertyTitle* SelectedPropertyTitle;
	
	UPROPERTY(meta = (BindWidget))
	UButton* BackBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* BuildBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* MortgageBtn;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MortgageBtnText;

	UPROPERTY(meta = (BindWidget))
	UWrapBox* PropertyWrapBox;

	UPROPERTY(meta = (BindWidget))
	UBLPUWPropertyDetails* PropertyDetails;

	// PropertyTitle class reference
	TSubclassOf<UUserWidget> PropertyTitleClass;

	UFUNCTION()
	void BackBtnClicked();
	
	UFUNCTION()
	void BuildBtnClicked();

	UFUNCTION()
	void MortgageBtnClicked();

	UFUNCTION()
	void ItsMyTurn();

	UFUNCTION()
	void ItsNotMyTurn();
};
