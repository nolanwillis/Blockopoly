// Fill out your copyright notice in the Description page of Project Settings. 
#include "BLPSpace.h"
#include "../../Framework/State/BLPGameState.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABLPSpace::ABLPSpace()
{ // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	RootComponent = Root;

	Square = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Square"));
	Square->SetupAttachment(RootComponent);

	SpawnPoint0 = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint0"));
	SpawnPoint0->SetupAttachment(RootComponent);
	SpawnPoint1 = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint1"));
	SpawnPoint1->SetupAttachment(RootComponent);
	SpawnPoint2 = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint2"));
	SpawnPoint2->SetupAttachment(RootComponent);
	SpawnPoint3 = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint3"));
	SpawnPoint3->SetupAttachment(RootComponent);
}

USceneComponent* ABLPSpace::GetSpawnPoint(const int& BLPPlayerId) {
	switch (BLPPlayerId)
	{
	case 0:
		return SpawnPoint0;
	case 1:
		return SpawnPoint1;
	case 2:
		return SpawnPoint2;
	case 3:
		return SpawnPoint3;
	default:
		return SpawnPoint0;
	}
}

// Called when the game starts or when spawned.
void ABLPSpace::BeginPlay()
{
	Super::BeginPlay();

	// Add self to SpaceList in GameState
	ABLPGameState* BLPGameStatePtr = Cast<ABLPGameState>(GetWorld()->GetGameState());
	if (BLPGameStatePtr)
	{
		BLPGameStatePtr->AddToSpaceList(this);
	}
}



