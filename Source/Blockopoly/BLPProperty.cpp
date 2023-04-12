// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPProperty.h"
#include "BLPGameState.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"

// Sets default values
ABLPProperty::ABLPProperty()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxCollider;
	SpawnPoint0 = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint0"));
	SpawnPoint1 = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint1"));
	SpawnPoint2 = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint2"));
	SpawnPoint3 = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint3"));
	TitleBar = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TitleBar"));

	SpawnPoint0->SetupAttachment(RootComponent);
	SpawnPoint1->SetupAttachment(RootComponent);
	SpawnPoint2->SetupAttachment(RootComponent);
	SpawnPoint3->SetupAttachment(RootComponent);
	TitleBar->SetupAttachment(RootComponent);

	
}

// Called when the game starts or when spawned
void ABLPProperty::BeginPlay()
{
	Super::BeginPlay();

	if (TitleBar)
	{
		TitleBar->SetMaterial(0, Color);
	}

	// Add self to property list in game state
	ABLPGameState* GameState = Cast<ABLPGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->PropertyList.Add(this);
	}
}

// Called every frame
void ABLPProperty::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

