// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPSpace.h"
#include "../../Framework/State/BLPGameState.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABLPSpace::ABLPSpace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	RootComponent = Root;
	Square = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Square"));
	Square->SetupAttachment(RootComponent);
}

FSpawnPoint* ABLPSpace::GetOpenSpawnPoint()
{
	// Find the first open spawn point and return it. We don't need a case for when all are taken because there
	// will always be enough spawn points.
	for (const auto SpawnPoint : SpawnPoints)
	{
		if (!SpawnPoint->Taken)
		{
			SpawnPoint->Taken = true;
			return SpawnPoint;
		}
	}
	return nullptr;
}

// Called when the game starts or when spawned.
void ABLPSpace::BeginPlay()
{
	Super::BeginPlay();

	// Set the location of spawn points
	SpawnPoint0.Transform.SetLocation(FVector(0, -75, 20));
	SpawnPoint0.Index = 0;
	SpawnPoint0.Taken = false;
	SpawnPoint1.Transform.SetLocation(FVector(0, 75, 20));
	SpawnPoint1.Index = 1;
	SpawnPoint1.Taken = false;
	SpawnPoint2.Transform.SetLocation(FVector(-110, -75, 20));
	SpawnPoint2.Index = 2;
	SpawnPoint2.Taken = false;
	SpawnPoint3.Transform.SetLocation(FVector(-110, 75, 20));
	SpawnPoint3.Index = 3;
	SpawnPoint3.Taken = false;

	// Add self to SpaceList in GameState
	ABLPGameState* GameState = Cast<ABLPGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->AddToSpaceList(this);
	}
}

// Called every frame
void ABLPSpace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

