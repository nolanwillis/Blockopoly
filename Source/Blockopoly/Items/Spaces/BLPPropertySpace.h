// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BLPSpace.h"
#include "BLPPropertySpace.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API ABLPPropertySpace : public ABLPSpace
{
	GENERATED_BODY()

public:
	ABLPPropertySpace();

	// Accessors
	int32 GetOwnerID() const { return OwnerID; }
	void SetOwnerID(const int32& Value) { OwnerID = Value; }
	int GetCurrentRent() const { return CurrentRent; }
	void SetCurrentRent(const int& Value) { CurrentRent = Value; }
	int GetBaseRent() const { return BaseRent; }
	int GetPurchaseCost() const { return PurchaseCost; }
	void SetPurchaseCost (const int& Value) { PurchaseCost = Value; }
	int GetMortgageValue() const { return MortgageValue; }
	void SetMortgageValue(const int& Value) { MortgageValue = Value; }
	bool GetIsMortgaged() const { return IsMortgaged; }
	void SetIsMortgaged(const bool& Value) { IsMortgaged = Value; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	// PlayerID of the player the owns this property, -1 if unowned
	UPROPERTY(Replicated, EditAnywhere, Category = PropertyData, meta = (AllowPrivateAccess = true))
	int32 OwnerID = -1;

	UPROPERTY(EditAnywhere, Category = PropertyData, meta = (AllowPrivateAccess = true))
	int BaseRent;
	
	UPROPERTY()
	int CurrentRent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = PropertyData, meta = (AllowPrivateAccess = true))
	int PurchaseCost;
	
	UPROPERTY(EditAnywhere, Category = PropertyData, meta = (AllowPrivateAccess = true))
	int MortgageValue;

	UPROPERTY()
	bool IsMortgaged;
};

