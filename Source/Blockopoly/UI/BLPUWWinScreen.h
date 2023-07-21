// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./BLPUserWidget.h"
#include "BLPUWWinScreen.generated.h"

class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API UBLPUWWinScreen : public UBLPUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WinnerText;

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	UButton* ExitBtn;
	
	UFUNCTION()
	void ExitBtnClicked();
};
