// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./BLPUserWidget.h"
#include "./Components/TimelineComponent.h"
#include "BLPUWNotification.generated.h"

class UTextBlock;
class USizeBox;
class UCurveFloat;
class UBLPUWGameMenu;

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API UBLPUWNotification : public UBLPUserWidget
{
	GENERATED_BODY()
public:
	void Setup(const FString& InType, const FString& Heading, const FString& Description, UBLPUWGameMenu* InParent);
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeConstruct() override;
	
private:
	UPROPERTY()
	FString Type;
	
	UPROPERTY()
	UBLPUWGameMenu* Parent;
	
	UPROPERTY(meta = (BindWidget))
	USizeBox* MainSizeBox;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HeadingTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DescriptionTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TitleTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline Curves", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* TitleOpacityCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline Curves", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* MessageOpacityCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline Curves", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* CardStartOpacityCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline Curves", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* CardEndOpacityCurve;

	FTimeline TitleTimeline;
	FTimeline MessageTimeline;
	FTimeline CardStartTimeline;
	FTimeline CardEndTimeline;

	UFUNCTION()
	void TitleOpacityUpdate(float Value);

	UFUNCTION()
	void MessageOpacityUpdate(float Value);

	UFUNCTION()
	void CardOpacityUpdate(float Value);

	UFUNCTION()
	void TitleOpacityFinished();

	UFUNCTION()
	void MessageOpacityFinished();

	UFUNCTION()
	void CardStartOpacityFinished();

	UFUNCTION()
	void CardEndOpacityFinished();
};
