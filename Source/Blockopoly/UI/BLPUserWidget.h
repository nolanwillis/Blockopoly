// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BLPUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API UBLPUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Setup();
	virtual void Remove();
};
