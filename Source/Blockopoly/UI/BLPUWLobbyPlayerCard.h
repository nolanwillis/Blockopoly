// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./BLPUWPlayerCard.h"
#include "BLPUWLobbyPlayerCard.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API UBLPUWLobbyPlayerCard : public UBLPUWPlayerCard
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UImage* CheckImage;
	
};
