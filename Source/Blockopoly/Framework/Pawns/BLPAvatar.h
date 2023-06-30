// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BLPAvatar.generated.h"

class ABLPCameraManager;
class UBoxComponent;

UCLASS()
class BLOCKOPOLY_API ABLPAvatar : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABLPAvatar();

private:
	UPROPERTY(EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
	UBoxComponent* BoxCollider;
	
	UPROPERTY(EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* Shape;

	UPROPERTY()
	ABLPCameraManager* CameraManager;
};
