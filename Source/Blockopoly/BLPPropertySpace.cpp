// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPPropertySpace.h"
#include "BLPGameState.h"

#include "Materials/Material.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

ABLPPropertySpace::ABLPPropertySpace()
{
	bReplicates = true;
	TitleBar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TitleBar"));
	TitleBar->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABLPPropertySpace::BeginPlay()
{
	Super::BeginPlay();

	if (TitleBar) TitleBar->SetMaterial(0, Color);

	if (const UWorld* World = GetWorld())
	{
		if (ABLPGameState* GameStatePtr = Cast<ABLPGameState>(World->GetGameState()))
		{
			GameStatePtr->AddToAvailablePropertySpaceList(this);
		}
	}
}

void ABLPPropertySpace::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Here we list the variables we want to replicate
	DOREPLIFETIME(ABLPPropertySpace, OwnerID);
}

