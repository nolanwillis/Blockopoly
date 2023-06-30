// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPAvatar.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ABLPAvatar::ABLPAvatar()
{
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	RootComponent = BoxCollider;

	Shape = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shape"));
	Shape->SetupAttachment(RootComponent);

	if (HasAuthority())
	{
		SetReplicates(true);
		AActor::SetReplicateMovement(true);
	}
}

