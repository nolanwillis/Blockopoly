// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BLPSpace.generated.h"

class USceneComponent; 
class UStaticMeshComponent;

USTRUCT()
struct FSpawnPoint
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FTransform Transform;

	bool Taken = false;
};

UCLASS()
class BLOCKOPOLY_API ABLPSpace : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABLPSpace();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Accessors
	FString GetName() { return Name; }
	int GetSpaceID() { return SpaceID; }

	// Gets the transform of an open SpawnPoint from the SpawnPoints array
	FTransform GetSpawnPointTransform();

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

	// Array of spawn points
	TArray<FSpawnPoint> SpawnPoints{ SpawnPoint0, SpawnPoint1, SpawnPoint2, SpawnPoint3 };

	UPROPERTY(EditAnywhere, Category = SpawnPoints, meta = (AllowPrivateAccess = true))
		FSpawnPoint SpawnPoint0;

	UPROPERTY(EditAnywhere, Category = SpawnPoints, meta = (AllowPrivateAccess = true))
		FSpawnPoint SpawnPoint1;

	UPROPERTY(EditAnywhere, Category = SpawnPoints, meta = (AllowPrivateAccess = true))
		FSpawnPoint SpawnPoint2;

	UPROPERTY(EditAnywhere, Category = SpawnPoints, meta = (AllowPrivateAccess = true))
		FSpawnPoint SpawnPoint3;

	

	
};
