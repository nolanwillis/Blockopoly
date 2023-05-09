// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPPropertySpace.h"
#include "BLPGameState.h"

#include "Materials/Material.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

ABLPPropertySpace::ABLPPropertySpace()
{
	bReplicates = true;
	
}

// Called when the game starts or when spawned
void ABLPPropertySpace::BeginPlay()
{
	Super::BeginPlay();

	if (const UWorld* WorldPtr = GetWorld())
	{
		if (ABLPGameState* GameStatePtr = Cast<ABLPGameState>(WorldPtr->GetGameState()))
		{
			GameStatePtr->AddToAvailablePropertySpaceList(this);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GameStatePtr is null, from PropertySpace"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WorldPtr is null, from PropertySpace"));
	}
}

void ABLPPropertySpace::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Here we list the variables we want to replicate
	DOREPLIFETIME(ABLPPropertySpace, OwnerID);
}

