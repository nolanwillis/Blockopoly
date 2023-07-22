// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BLPSpawnPlatform.generated.h"

UCLASS()
class BLOCKOPOLY_API ABLPSpawnPlatform : public AActor
{
	GENERATED_BODY()
	
public:
	ABLPSpawnPlatform();

	int GetPlatformId() const { return PlatformId; }

	void ToggleLight(const int& Id, const bool& TurnOn) const;

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))	
	int PlatformId;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))	
	UStaticMeshComponent* Platform;
   
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))	
	UMaterial* M_Player0;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))	
	UMaterial* M_Player1; 
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))	
	UMaterial* M_Player2; 
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))	
	UMaterial* M_Player3;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))	
	UMaterial* M_Off;
};


