// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BLPPropertySpace.h"
#include "BLPEstatePropertySpace.generated.h"

class UMaterial;
class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API ABLPEstatePropertySpace : public ABLPPropertySpace
{
	GENERATED_BODY()
	
public:
	ABLPEstatePropertySpace();

	int GetBuildingCount() const { return BuildingCount; }
	void SetBuildingCount(const int& Value) { if (Value < 0 || Value > 5) return; BuildingCount = Value; }
	int GetBuildingCost() const { return BuildingCost; }
	UStaticMeshComponent* GetHouse0() const {return House0;}
	UStaticMeshComponent* GetHouse1() const {return House1;}
	UStaticMeshComponent* GetHouse2() const {return House2;}
	UStaticMeshComponent* GetHouse3() const {return House3;}
	UStaticMeshComponent* GetHotel() const {return Hotel;}

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, Category = EstateData, meta = (AllowPrivateAccess = true))
	UMaterial* Color;

	UPROPERTY(EditAnywhere, Category = EstateData, meta = (AllowPrivateAccess = true))
	int BuildingCost;
	
	UPROPERTY(EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* TitleBar;
	
	UPROPERTY(EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* House0;

	UPROPERTY(EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* House1;

	UPROPERTY(EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* House2;

	UPROPERTY(EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* House3;

	UPROPERTY(EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* Hotel;
	
	UPROPERTY(Replicated)
	int BuildingCount = 0;

	
	
};
