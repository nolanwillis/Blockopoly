// Fill out your copyright notice in the Description page of Project Settings.


#include "./BLPUWDrawCardMessage.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

void UBLPUWDrawCardMessage::Setup(const FString& Heading, const FString& Description) const
{
	CardHeading->SetText(FText::FromString(Heading));
	CardDescription->SetText(FText::FromString(Description));
}

void UBLPUWDrawCardMessage::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CardTitleTimeline.TickTimeline(InDeltaTime);
	MessageTimeline.TickTimeline(InDeltaTime);
	CardStartTimeline.TickTimeline(InDeltaTime);
	CardEndTimeline.TickTimeline(InDeltaTime);
}

void UBLPUWDrawCardMessage::NativeConstruct()
{
	Super::NativeConstruct();

	FOnTimelineFloat CardTitleOpacityProgressUpdate;
	CardTitleOpacityProgressUpdate.BindUFunction(this, FName("CardTitleOpacityUpdate"));
	FOnTimelineFloat MessageOpacityProgressUpdate;
	MessageOpacityProgressUpdate.BindUFunction(this, FName("MessageOpacityUpdate"));
	FOnTimelineFloat CardOpacityProgressUpdate;
	CardOpacityProgressUpdate.BindUFunction(this, FName("CardOpacityUpdate"));
	FOnTimelineEvent CardTitleOpacityFinishedEvent;
	
	CardTitleOpacityFinishedEvent.BindUFunction(this, FName("CardTitleOpacityFinished"));
	FOnTimelineEvent MessageOpacityFinishedEvent;
	MessageOpacityFinishedEvent.BindUFunction(this, FName("MessageOpacityFinished"));
	FOnTimelineEvent CardStartOpacityFinishedEvent;
	CardStartOpacityFinishedEvent.BindUFunction(this, FName("CardStartOpacityFinished"));
	FOnTimelineEvent CardEndOpacityFinishedEvent;
	CardEndOpacityFinishedEvent.BindUFunction(this, FName("CardEndOpacityFinished"));

	if (!CardTitleOpacityCurve || !MessageOpacityCurve || !CardStartOpacityCurve || !CardEndOpacityCurve)
	{
		UE_LOG(LogTemp, Warning, TEXT("BLPUWDrawCardMessage: all timeline curves are not set"));
		return;
	}
	
	CardTitleTimeline.AddInterpFloat(CardTitleOpacityCurve, CardTitleOpacityProgressUpdate);
	MessageTimeline.AddInterpFloat(MessageOpacityCurve, MessageOpacityProgressUpdate);
	CardStartTimeline.AddInterpFloat(CardStartOpacityCurve, CardOpacityProgressUpdate);
	CardEndTimeline.AddInterpFloat(CardEndOpacityCurve, CardOpacityProgressUpdate);
	
	CardTitleTimeline.SetTimelineFinishedFunc(CardTitleOpacityFinishedEvent);
	MessageTimeline.SetTimelineFinishedFunc(MessageOpacityFinishedEvent);
	CardStartTimeline.SetTimelineFinishedFunc(CardStartOpacityFinishedEvent);
	CardEndTimeline.SetTimelineFinishedFunc(CardEndOpacityFinishedEvent);

	CardStartTimeline.PlayFromStart();
}

void UBLPUWDrawCardMessage::CardTitleOpacityUpdate(float Value)
{
	const float NewOpacity = FMath::Lerp(0.0, 1.0, Value);
	CardTitle->SetRenderOpacity(NewOpacity);
}

void UBLPUWDrawCardMessage::MessageOpacityUpdate(float Value)
{
	const float NewOpacity = FMath::Lerp(0.0, 1.0, Value);
	CardHeading->SetRenderOpacity(NewOpacity);
	CardDescription->SetRenderOpacity(NewOpacity);
}

void UBLPUWDrawCardMessage::CardOpacityUpdate(float Value)
{
	const float NewOpacity = FMath::Lerp(0.0, 1.0, Value);
	MainSizeBox->SetRenderOpacity(Value);
}

void UBLPUWDrawCardMessage::CardTitleOpacityFinished()
{
	MessageTimeline.PlayFromStart();
	CardTitle->SetVisibility(ESlateVisibility::Hidden);
}

void UBLPUWDrawCardMessage::MessageOpacityFinished()
{
	CardEndTimeline.PlayFromStart();
}

void UBLPUWDrawCardMessage::CardStartOpacityFinished()
{
	CardTitleTimeline.PlayFromStart();
}

void UBLPUWDrawCardMessage::CardEndOpacityFinished()
{
	this->RemoveFromParent();
}
