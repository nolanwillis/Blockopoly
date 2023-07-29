// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BLPSpace.generated.h"

class USceneComponent; 
class UStaticMeshComponent;

UCLASS()
class BLOCKOPOLY_API ABLPSpace : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABLPSpace();
	
	// Accessors
	FString GetName() { return Name; }
	int GetSpaceID() const { return SpaceID; }

	// Gets the transform of an open SpawnPoint from the SpawnPoints array
	USceneComponent* GetSpawnPoint(const int& BLPPlayerId);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SpaceData, meta = (AllowPrivateAccess = true))
	FString Name;

	UPROPERTY(EditAnywhere, Category = SpaceData, meta = (AllowPrivateAccess = true))
	int SpaceID;

	UPROPERTY(EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* Square;
	
	UPROPERTY(EditAnywhere, Category = Spawning, meta = (AllowPrivateAccess = true, MakeEditWidget = true))
	USceneComponent* SpawnPoint0;
	UPROPERTY(EditAnywhere, Category = Spawning, meta = (AllowPrivateAccess = true, MakeEditWidget = true))
	USceneComponent* SpawnPoint1;
	UPROPERTY(EditAnywhere, Category = Spawning, meta = (AllowPrivateAccess = true, MakeEditWidget = true))
	USceneComponent* SpawnPoint2;
	UPROPERTY(EditAnywhere, Category = Spawning, meta = (AllowPrivateAccess = true, MakeEditWidget = true))
	USceneComponent* SpawnPoint3;
	
	// Array of spawn points
	TArray<USceneComponent*> SpawnPoints{ SpawnPoint0, SpawnPoint1, SpawnPoint2, SpawnPoint3 };



	

	
};
