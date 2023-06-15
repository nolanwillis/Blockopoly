// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./BLPUserWidget.h"
#include "BLPUWPropertyMenu.generated.h"

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
	
	void RefreshPropertyDetails(ABLPPropertySpace* PropertySpace) const;


protected:
	virtual bool Initialize();

private:
	UPROPERTY(meta = (BindWidget))
	UButton* BackBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* BuildBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* MortgageBtn;

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
