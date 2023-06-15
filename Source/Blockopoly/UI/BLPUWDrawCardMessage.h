// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./BLPUserWidget.h"
#include "./Components/TimelineComponent.h"
#include "BLPUWDrawCardMessage.generated.h"

class UTextBlock;
class USizeBox;
class UCurveFloat;

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API UBLPUWDrawCardMessage : public UBLPUserWidget
{
	GENERATED_BODY()

public:
	void Setup(const FString& Heading, const FString& Description) const;
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	USizeBox* MainSizeBox;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CardHeading;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CardDescription;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CardTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline Curves", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* CardTitleOpacityCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline Curves", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* MessageOpacityCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline Curves", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* CardStartOpacityCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline Curves", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* CardEndOpacityCurve;

	FTimeline CardTitleTimeline;
	FTimeline MessageTimeline;
	FTimeline CardStartTimeline;
	FTimeline CardEndTimeline;

	UFUNCTION()
	void CardTitleOpacityUpdate(float Value);

	UFUNCTION()
	void MessageOpacityUpdate(float Value);

	UFUNCTION()
	void CardOpacityUpdate(float Value);

	UFUNCTION()
	void CardTitleOpacityFinished();

	UFUNCTION()
	void MessageOpacityFinished();

	UFUNCTION()
	void CardStartOpacityFinished();

	UFUNCTION()
	void CardEndOpacityFinished();
};
