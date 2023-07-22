// Fill out your copyright notice in the Description page of Project Settings.


#include "./BLPSpawnPlatform.h"

#include "Blockopoly/Framework/State/BLPGameState.h"

// Sets default values
ABLPSpawnPlatform::ABLPSpawnPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Platform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	Platform->SetupAttachment(RootComponent);
}

void ABLPSpawnPlatform::ToggleLight(const int& Id, const bool& TurnOn) const
{
	if (!TurnOn) { Platform->SetMaterial(0, M_Off); return; }
	
	switch (Id)
	{
	case 0:
		Platform->SetMaterial(0, M_Player0);
		break;
	case 1:
		Platform->SetMaterial(0, M_Player1);
		break;
	case 2:
		Platform->SetMaterial(0, M_Player2);
		break;
	case 3:
		Platform->SetMaterial(0, M_Player3);
		break;
	default:
		break;
	}
}

void ABLPSpawnPlatform::BeginPlay()
{
	Super::BeginPlay();

	const UWorld* World = GetWorld();
	if (!World) return;
	ABLPGameState* BLPGameStatePtr = Cast<ABLPGameState>(World->GetGameState());

	BLPGameStatePtr->AddToLobbySpawnPlatformArray(this);
}

