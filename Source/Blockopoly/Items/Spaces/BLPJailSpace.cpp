// Fill out your copyright notice in the Description page of Project Settings

#include "./BLPJailSpace.h"
#include "Components/SceneComponent.h"

ABLPJailSpace::ABLPJailSpace()
{
	JailCell0 = CreateDefaultSubobject<USceneComponent>(TEXT("JailCell0"));
	JailCell0->SetupAttachment(RootComponent);
	JailCell1 = CreateDefaultSubobject<USceneComponent>(TEXT("JailCell1"));
	JailCell1->SetupAttachment(RootComponent);
	JailCell2 = CreateDefaultSubobject<USceneComponent>(TEXT("JailCell2"));
	JailCell2->SetupAttachment(RootComponent);
	JailCell3 = CreateDefaultSubobject<USceneComponent>(TEXT("JailCell3"));
	JailCell3->SetupAttachment(RootComponent);
}

USceneComponent* ABLPJailSpace::GetJailCell(const int& BLPPlayerId)
{
	switch (BLPPlayerId)
	{
	case 0:
		return JailCell0;
	case 1:
		return JailCell1;
	case 2:
		return JailCell2;
	case 3:
		return JailCell3;
	default:
		return JailCell0;
	}
}
