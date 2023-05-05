// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BLPPropertySpace.h"
#include "GameFramework/PlayerState.h"
#include "BLPPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API ABLPPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABLPPlayerState();
	int GetBalance() const { return CreditBalance; }
	void AddToBalance(const int& Value) { CreditBalance = CreditBalance+Value > 0 ? CreditBalance+Value : 0; }
	int GetDesiredSpaceID() const { return DesiredSpaceID; }
	void SetDesiredSpaceID(const int& Value) { DesiredSpaceID = Value; }
	bool GetIsItMyTurn() const { return IsItMyTurn; }
	void SetIsItMyTurn(const bool& Value) { IsItMyTurn = Value; }
	TArray<ABLPPropertySpace*> GetOwnedPropertyList() const { return OwnedPropertyList; }
	void AddToOwnedPropertyList(ABLPPropertySpace* Value) { OwnedPropertyList.Add(Value); }
	void RemoveFromOwnedPropertyList(ABLPPropertySpace* Value) { OwnedPropertyList.Remove(Value); }

private:
	UPROPERTY(ReplicatedUsing=OnRep_CreditBalance)
	int CreditBalance = 1500;
	
	UPROPERTY(ReplicatedUsing=OnRep_DesiredSpaceID)
	int DesiredSpaceID = 0;
	
	UPROPERTY(ReplicatedUsing=OnRep_IsItMyTurn)
	bool IsItMyTurn = false;

	UPROPERTY(ReplicatedUsing=OnRep_OwnedPropertyList)
	TArray<ABLPPropertySpace*> OwnedPropertyList;

	UFUNCTION()
	void OnRep_CreditBalance() const;
	
	UFUNCTION()
	void OnRep_DesiredSpaceID() const;

	UFUNCTION()
	void OnRep_IsItMyTurn() const;

	UFUNCTION()
	void OnRep_OwnedPropertyList() const;
	
};
