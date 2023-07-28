// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPEstatePropertySpace.h"

#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

ABLPEstatePropertySpace::ABLPEstatePropertySpace()
{
	TitleBar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TitleBar"));
	House0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("House0"));
	House1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("House1"));
	House2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("House2"));
	House3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("House3"));
	Hotel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hotel"));
	
	TitleBar->SetupAttachment(RootComponent);
	House0->SetupAttachment(RootComponent);
	House1->SetupAttachment(RootComponent);
	House2->SetupAttachment(RootComponent);
	House3->SetupAttachment(RootComponent);
	Hotel->SetupAttachment(RootComponent);

	SetReplicates(true);
}

void ABLPEstatePropertySpace::BeginPlay()
{
	Super::BeginPlay();

	if (TitleBar) TitleBar->SetMaterial(0, Color);

	House0->SetVisibility(false, false);
	House1->SetVisibility(false, false);
	House2->SetVisibility(false, false);
	House3->SetVisibility(false, false);
	Hotel->SetVisibility(false, false);
}

void ABLPEstatePropertySpace::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Here we list the variables we want to replicate
	DOREPLIFETIME(ABLPEstatePropertySpace, CanBuild);
}

