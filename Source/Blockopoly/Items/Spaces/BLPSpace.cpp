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

	// Set the location of spawn points
	SpawnPoint0.Transform.SetLocation(FVector(0, -60, 0));
	SpawnPoint1.Transform.SetLocation(FVector(0, 60, 0));
	SpawnPoint2.Transform.SetLocation(FVector(-100, -60, 0));
	SpawnPoint3.Transform.SetLocation(FVector(-100, 60, 0));
}

FTransform ABLPSpace::GetSpawnPointTransform()
{
	// Find the first open spawn point and return it. We don't need a case for when all are taken because there
	// will always be enough spawn points.
	FTransform Response;
	for (auto& SpawnPoint : SpawnPoints)
	{
		if (!SpawnPoint.Taken)
		{
			SpawnPoint.Taken = true;
			Response = SpawnPoint.Transform;
		}
	}
	return Response;
}

// Called when the game starts or when spawned.
void ABLPSpace::BeginPlay()
{
	Super::BeginPlay();

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


