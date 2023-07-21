// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./BLPUserWidget.h"
#include "BLPUWPlayerCard.generated.h"

class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API UBLPUWPlayerCard : public UBLPUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UButton* Container;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;
};
