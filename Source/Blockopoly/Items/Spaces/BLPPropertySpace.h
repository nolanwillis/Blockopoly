// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BLPSpace.h"
#include "BLPPropertySpace.generated.h"

class UMaterial;

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
	int32 GetOwnerId() const { return OwnerId; }
	void SetOwnerId(const int32& Value) { OwnerId = Value; OnRep_OwnerId(); }
	int GetCurrentRent() const { return CurrentRent; }
	void SetCurrentRent(const int& Value) { CurrentRent = Value; }
	int GetBaseRent() const { return BaseRent; }
	int GetPurchaseCost() const { return PurchaseCost; }
	void SetPurchaseCost (const int& Value) { PurchaseCost = Value; }
	int GetMortgageValue() const { return MortgageValue; }
	void SetMortgageValue(const int& Value){ MortgageValue = Value; }
	bool GetIsMortgaged() const { return IsMortgaged; }
	void SetIsMortgaged(const bool& Value){ IsMortgaged = Value; OnRep_IsMortgaged(); }

	bool GetHasPendingSale() const { return HasPendingSale; }
	void SetHasPendingSale(const bool& Value){ HasPendingSale = Value; }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(Replicated, EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* PerimeterTop;
	UPROPERTY(Replicated, EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* PerimeterBottom;
	UPROPERTY(Replicated, EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* PerimeterLeft;
	UPROPERTY(Replicated, EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* PerimeterRight;
	
	// PlayerID of the player the owns this property, -1 if unowned
	UPROPERTY(ReplicatedUsing=OnRep_OwnerId, EditAnywhere, Category = Ownership, meta = (AllowPrivateAccess = true))
	int32 OwnerId = -1;

	UPROPERTY(EditAnywhere, Category = Ownership, meta = (AllowPrivateAccess = true))
	UMaterial* M_Player0;
	UPROPERTY(EditAnywhere, Category = Ownership, meta = (AllowPrivateAccess = true))
	UMaterial* M_Player1;
	UPROPERTY(EditAnywhere, Category = Ownership, meta = (AllowPrivateAccess = true))
	UMaterial* M_Player2;
	UPROPERTY(EditAnywhere, Category = Ownership, meta = (AllowPrivateAccess = true))
	UMaterial* M_Player3;
	UPROPERTY(EditAnywhere, Category = Ownership, meta = (AllowPrivateAccess = true))
	UMaterial* M_Mortgaged;
	UPROPERTY(EditAnywhere, Category = Ownership, meta = (AllowPrivateAccess = true))
	UMaterial* M_Unowned;

	UPROPERTY(EditAnywhere, Category = Financials, meta = (AllowPrivateAccess = true))
	int BaseRent;
	
	UPROPERTY()
	int CurrentRent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Financials, meta = (AllowPrivateAccess = true))
	int PurchaseCost;
	
	UPROPERTY(EditAnywhere, Category = Financials, meta = (AllowPrivateAccess = true))
	int MortgageValue;

	UPROPERTY(ReplicatedUsing=OnRep_IsMortgaged)
	bool IsMortgaged = false;

	UPROPERTY(Replicated)
	bool HasPendingSale = false;

	UFUNCTION()
	void OnRep_OwnerId();

	UFUNCTION()
	void OnRep_IsMortgaged();

	void SetPerimeterColor(const int& Value);
};

