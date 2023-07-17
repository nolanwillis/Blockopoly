// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BLPCameraManager.generated.h"

UCLASS()
class BLOCKOPOLY_API ABLPCameraManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AActor* GetCamera(const int& Index) const;
	
	UPROPERTY(EditAnywhere)
	AActor* Camera0;
	UPROPERTY(EditAnywhere)
	AActor* Camera1;
	UPROPERTY(EditAnywhere)
	AActor* Camera2;
	UPROPERTY(EditAnywhere)
	AActor* Camera3;
	
};
