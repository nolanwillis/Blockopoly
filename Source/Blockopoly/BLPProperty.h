// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BLPProperty.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UBoxComponent;
class UMaterial;

UCLASS()
class BLOCKOPOLY_API ABLPProperty : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABLPProperty();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Array of spawn points
	TArray<USceneComponent*> SpawnPoints = {SpawnPoint0, SpawnPoint1, SpawnPoint2, SpawnPoint3};
	
	// Accessors
	void SetOwnerID(int Value) { OwnerID = Value; }
	void SetSellValue(int Value) { SellValue = Value; }
	void SetRent(int Value) { Rent = Value; }
	FString GetName() { return Name; }
	int GetOwnerID() { return OwnerID; }
	int GetSellValue(int Value) { return SellValue; }
	int GetRent() { return Rent; }
	int GetOrderID() { return OrderID; }
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = PropertyData, meta = (AllowPrivateAccess = true))
	FString Name;

	UPROPERTY(EditAnywhere, Category = PropertyData, meta = (AllowPrivateAccess = true))
	int OrderID;
	
	UPROPERTY(EditAnywhere, Category = PropertyData, meta = (AllowPrivateAccess = true))
	int OwnerID = -1;
	
	UPROPERTY(EditAnywhere, Category = PropertyData, meta = (AllowPrivateAccess = true))
	int Rent;
	
	UPROPERTY(EditAnywhere, Category = PropertyData, meta = (AllowPrivateAccess = true))
	int SellValue;

	UPROPERTY(EditAnywhere, Category = PropertyData, meta = (AllowPrivateAccess = true))
	UMaterial* Color;

	UPROPERTY(EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
	UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
		UStaticMeshComponent* TitleBar;
	
	UPROPERTY(EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
	USceneComponent* SpawnPoint0;
	
	UPROPERTY(EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
	USceneComponent* SpawnPoint1;
	
	UPROPERTY(EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
	USceneComponent* SpawnPoint2;
	
	UPROPERTY(EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
	USceneComponent* SpawnPoint3;

};
