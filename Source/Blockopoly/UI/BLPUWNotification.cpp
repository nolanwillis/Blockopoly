// Fill out your copyright notice in the Description page of Project Settings.

#include "./BLPUWNotification.h"
#include "BLPUWGameMenu.h"
#include "Blockopoly/Framework/BLPPlayerController.h"
#include "Blockopoly/Framework/Pawns/BLPAvatar.h"
#include "Blockopoly/Framework/State/BLPPlayerState.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

void UBLPUWNotification::Setup(const FString& InType, const FString& Heading, const FString& Description, UBLPUWGameMenu* InParent)
{
	HeadingTextBlock->SetText(FText::FromString(Heading));
	DescriptionTextBlock->SetText(FText::FromString(Description));
	Parent = InParent;
	Type = InType;
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
	
	if (GetOwningPlayerState<ABLPPlayerState>()->GetIsItMyTurn())
	{
		UWorld* World = GetWorld();
		ABLPGameState* BLPGameStatePtr = Cast<ABLPGameState>(World->GetGameState());
		ABLPPlayerState* BLPPlayerStatePtr = GetOwningPlayerState<ABLPPlayerState>();
		ABLPPlayerController* BLPPlayerControllerPtr = GetOwningPlayer<ABLPPlayerController>();
		if (!BLPPlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWNotification: BLPPlayerControllerPtr is null")); return; }
		if (!BLPGameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWNotification: BLPGameStatePtr is null")); return; }
		if (!BLPPlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWNotification: BLPPlayerStatePtr is null")); return; }
		
		if (Type == "Roll")
		{
			ABLPAvatar* BLPAvatarPtr = GetOwningPlayerPawn<ABLPAvatar>();
			if (!BLPAvatarPtr) { UE_LOG(LogTemp, Warning, TEXT("BLPUWNotification: BLPAvatarPtr is null")); return; }
			BLPPlayerControllerPtr->Server_ReflectRollInGame(BLPAvatarPtr, BLPPlayerStatePtr, BLPGameStatePtr);
		}
		else if (Type == "Chance")
		{
			BLPPlayerControllerPtr->Server_ExecuteChanceCard(BLPPlayerStatePtr, BLPGameStatePtr);
		}
		else if (Type == "Community Chest")
		{
			BLPPlayerControllerPtr->Server_ExecuteChestCard(BLPPlayerStatePtr, BLPGameStatePtr);
		}
	}
}

