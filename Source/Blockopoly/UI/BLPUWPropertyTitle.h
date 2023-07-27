// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./BLPUserWidget.h"
#include "Blockopoly/Items/Spaces/BLPPropertySpace.h"
#include "BLPUWPropertyTitle.generated.h"

class ABLPPropertySpace;
class UBLPUWPropertyMenu;
class UButton;
class UBorder;
class UTextBlock;

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API UBLPUWPropertyTitle : public UBLPUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UBorder* PropertyTitleBorder;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PropertyNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	UButton* PropertyTitleContainer;

	// Method that sets up this session entry
	void Setup(UBLPUWPropertyMenu* InParent, ABLPPropertySpace* PropertySpacePtr);

private:
	UPROPERTY()
	UBLPUWPropertyMenu* Parent;

	UPROPERTY()
	ABLPPropertySpace* AssociatedProperty;

	UFUNCTION()
	void OnClick();
	
};
