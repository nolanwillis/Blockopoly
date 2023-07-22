// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPPropertySpace.h"
#include "../../Framework/State/BLPGameState.h"

#include "Materials/Material.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

ABLPPropertySpace::ABLPPropertySpace()
{
	bReplicates = true;

	PerimeterTop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PerimeterTop"));
	PerimeterBottom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PerimeterBottom"));
	PerimeterLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PerimeterLeft"));
	PerimeterRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PerimeterRight"));

	PerimeterTop->SetupAttachment(RootComponent);
	PerimeterBottom->SetupAttachment(RootComponent);
	PerimeterLeft->SetupAttachment(RootComponent);
	PerimeterRight->SetupAttachment(RootComponent);
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

	// Initialize the rent for the property to the value of BaseRent 
	CurrentRent = BaseRent;
}

void ABLPPropertySpace::OnRep_OwnerId()
{
	SetPerimeterColor(OwnerId);
}

void ABLPPropertySpace::OnRep_IsMortgaged()
{
	if (IsMortgaged)
	{
		SetPerimeterColor(4);
	}
	else
	{
		SetPerimeterColor(OwnerId);
	}
}

void ABLPPropertySpace::SetPerimeterColor(const int& Value)
{
	switch (Value)
	{
	case 0:
		PerimeterTop->SetMaterial(0, M_Player0);
		PerimeterBottom->SetMaterial(0, M_Player0);
		PerimeterLeft->SetMaterial(0, M_Player0);
		PerimeterRight->SetMaterial(0, M_Player0);
		break;
	case 1:
		PerimeterTop->SetMaterial(0, M_Player1);
		PerimeterBottom->SetMaterial(0, M_Player1);
		PerimeterLeft->SetMaterial(0, M_Player1);
		PerimeterRight->SetMaterial(0, M_Player1);
		break;
	case 2:
		PerimeterTop->SetMaterial(0, M_Player2);
		PerimeterBottom->SetMaterial(0, M_Player2);
		PerimeterLeft->SetMaterial(0, M_Player2);
		PerimeterRight->SetMaterial(0, M_Player2);
		break;
	case 3:
		PerimeterTop->SetMaterial(0, M_Player3);
		PerimeterBottom->SetMaterial(0, M_Player3);
		PerimeterLeft->SetMaterial(0, M_Player3);
		PerimeterRight->SetMaterial(0, M_Player3);
		break;
	case 4:
		PerimeterTop->SetMaterial(0, M_Mortgaged);
		PerimeterBottom->SetMaterial(0, M_Mortgaged);
		PerimeterLeft->SetMaterial(0, M_Player3);
		PerimeterRight->SetMaterial(0, M_Player3);
        		
	default:
		PerimeterTop->SetMaterial(0, M_Unowned);
		PerimeterBottom->SetMaterial(0, M_Unowned);
		PerimeterLeft->SetMaterial(0, M_Unowned);
		PerimeterRight->SetMaterial(0, M_Unowned);
		break;
	}
}

void ABLPPropertySpace::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Here we list the variables we want to replicate
	DOREPLIFETIME(ABLPPropertySpace, OwnerId);
	DOREPLIFETIME(ABLPPropertySpace, IsMortgaged);
	DOREPLIFETIME(ABLPPropertySpace, PerimeterTop);
	DOREPLIFETIME(ABLPPropertySpace, PerimeterBottom);
	DOREPLIFETIME(ABLPPropertySpace, PerimeterLeft);
	DOREPLIFETIME(ABLPPropertySpace, PerimeterRight);
}

