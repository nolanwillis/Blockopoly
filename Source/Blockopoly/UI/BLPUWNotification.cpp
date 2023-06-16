// Fill out your copyright notice in the Description page of Project Settings.


#include "./BLPUWNotification.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

void UBLPUWNotification::Setup(const FString& Heading, const FString& Description) const
{
	HeadingTextBlock->SetText(FText::FromString(Heading));
	DescriptionTextBlock->SetText(FText::FromString(Description));
}

void UBLPUWNotification::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	TitleTimeline.TickTimeline(InDeltaTime);
	MessageTimeline.TickTimeline(InDeltaTime);
	CardStartTimeline.TickTimeline(InDeltaTime);
	CardEndTimeline.TickTimeline(InDeltaTime);
}

void UBLPUWNotification::NativeConstruct()
{
	Super::NativeConstruct();

	FOnTimelineFloat TitleOpacityProgressUpdate;
	TitleOpacityProgressUpdate.BindUFunction(this, FName("TitleOpacityUpdate"));
	FOnTimelineFloat MessageOpacityProgressUpdate;
	MessageOpacityProgressUpdate.BindUFunction(this, FName("MessageOpacityUpdate"));
	FOnTimelineFloat CardOpacityProgressUpdate;
	CardOpacityProgressUpdate.BindUFunction(this, FName("CardOpacityUpdate"));

	FOnTimelineEvent TitleOpacityFinishedEvent;
	TitleOpacityFinishedEvent.BindUFunction(this, FName("TitleOpacityFinished"));
	FOnTimelineEvent MessageOpacityFinishedEvent;
	MessageOpacityFinishedEvent.BindUFunction(this, FName("MessageOpacityFinished"));
	FOnTimelineEvent CardStartOpacityFinishedEvent;
	CardStartOpacityFinishedEvent.BindUFunction(this, FName("CardStartOpacityFinished"));
	FOnTimelineEvent CardEndOpacityFinishedEvent;
	CardEndOpacityFinishedEvent.BindUFunction(this, FName("CardEndOpacityFinished"));

	if (!TitleOpacityCurve || !MessageOpacityCurve || !CardStartOpacityCurve || !CardEndOpacityCurve)
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPUWDrawCardMessage: all timeline curves are not set"));
		return;
	}
	
	TitleTimeline.AddInterpFloat(TitleOpacityCurve, TitleOpacityProgressUpdate);
	MessageTimeline.AddInterpFloat(MessageOpacityCurve, MessageOpacityProgressUpdate);
	CardStartTimeline.AddInterpFloat(CardStartOpacityCurve, CardOpacityProgressUpdate);
	CardEndTimeline.AddInterpFloat(CardEndOpacityCurve, CardOpacityProgressUpdate);
	
	TitleTimeline.SetTimelineFinishedFunc(TitleOpacityFinishedEvent);
	MessageTimeline.SetTimelineFinishedFunc(MessageOpacityFinishedEvent);
	CardStartTimeline.SetTimelineFinishedFunc(CardStartOpacityFinishedEvent);
	CardEndTimeline.SetTimelineFinishedFunc(CardEndOpacityFinishedEvent);

	CardStartTimeline.PlayFromStart();
}

void UBLPUWNotification::TitleOpacityUpdate(float Value)
{
	const float NewOpacity = FMath::Lerp(0.0, 1.0, Value);
	TitleTextBlock->SetRenderOpacity(NewOpacity);
}

void UBLPUWNotification::MessageOpacityUpdate(float Value)
{
	const float NewOpacity = FMath::Lerp(0.0, 1.0, Value);
	HeadingTextBlock->SetRenderOpacity(NewOpacity);
	DescriptionTextBlock->SetRenderOpacity(NewOpacity);
}

void UBLPUWNotification::CardOpacityUpdate(float Value)
{
	const float NewOpacity = FMath::Lerp(0.0, 1.0, Value);
	MainSizeBox->SetRenderOpacity(Value);
}

void UBLPUWNotification::TitleOpacityFinished()
{
	MessageTimeline.PlayFromStart();
	TitleTextBlock->SetVisibility(ESlateVisibility::Hidden);
}

void UBLPUWNotification::MessageOpacityFinished()
{
	CardEndTimeline.PlayFromStart();
}

void UBLPUWNotification::CardStartOpacityFinished()
{
	TitleTimeline.PlayFromStart();
}

void UBLPUWNotification::CardEndOpacityFinished()
{
	this->RemoveFromParent();
}
