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

	FString GetFamilyColor() const { return FamilyColor; }
	int GetBuildingCount() const { return BuildingCount; }
	void SetBuildingCount(const int& Value) { if (Value < 0 || Value > 5) return; BuildingCount = Value; }
	int GetBuildingCost() const { return BuildingCost; }
	void SetCanBuild(const bool& Value) { CanBuild = Value; }
	bool GetCanBuild() const { return CanBuild; }
	int GetFamilyRent() const { return FamilyRent; }
	int GetRent1Houses() const { return Rent1Houses; }
	int GetRent2Houses() const { return Rent2Houses; }
	int GetRent3Houses() const { return Rent3Houses; }
	int GetRent4Houses() const { return Rent4Houses; }
	int GetRentHotel() const { return RentHotel; }
	UMaterial* GetColor() const { return Color; }
	UStaticMeshComponent* GetHouse0() const {return House0;}
	UStaticMeshComponent* GetHouse1() const {return House1;}
	UStaticMeshComponent* GetHouse2() const {return House2;}
	UStaticMeshComponent* GetHouse3() const {return House3;}
	UStaticMeshComponent* GetHotel() const {return Hotel;}

protected:
	virtual void BeginPlay() override;
	
private:
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

	UPROPERTY(EditAnywhere, Category = EstateData, meta = (AllowPrivateAccess = true))
	FString FamilyColor;
	
	UPROPERTY(EditAnywhere, Category = EstateData, meta = (AllowPrivateAccess = true))
	UMaterial* Color;

	UPROPERTY(meta = (AllowPrivateAccess = true))
	bool CanBuild = false;
	
	UPROPERTY(EditAnywhere, Category = EstateData, meta = (AllowPrivateAccess = true))
	int BuildingCost;
	
	UPROPERTY(EditAnywhere, Category = EstateData, meta = (AllowPrivateAccess = true))
	int FamilyRent;

	UPROPERTY(EditAnywhere, Category = EstateData, meta = (AllowPrivateAccess = true))
	int Rent1Houses;

	UPROPERTY(EditAnywhere, Category = EstateData, meta = (AllowPrivateAccess = true))
	int Rent2Houses;

	UPROPERTY(EditAnywhere, Category = EstateData, meta = (AllowPrivateAccess = true))
	int Rent3Houses;

	UPROPERTY(EditAnywhere, Category = EstateData, meta = (AllowPrivateAccess = true))
	int Rent4Houses;

	UPROPERTY(EditAnywhere, Category = EstateData, meta = (AllowPrivateAccess = true))
	int RentHotel;

	UPROPERTY()
	int BuildingCount = 0;
	
};
