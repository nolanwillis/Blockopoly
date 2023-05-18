// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BLPPlayerState.generated.h"

class ABLPPropertySpace;
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
	
	int GetCurrentSpaceId() const { return CurrentSpaceId; }
	void SetCurrentSpaceId(const int& Value){ CurrentSpaceId = Value; }
	
	bool GetIsItMyTurn() const { return IsItMyTurn; }
	void SetIsItMyTurn(const bool& Value) { IsItMyTurn = Value; }
	
	TArray<ABLPPropertySpace*> GetOwnedPropertyList() const { return OwnedPropertyList; }
	void AddToOwnedPropertyList(ABLPPropertySpace* Value) { OwnedPropertyList.Add(Value); }
	void RemoveFromOwnedPropertyList(ABLPPropertySpace* Value) { OwnedPropertyList.Remove(Value); }
	
	int GetInJailTurnCounter() const { return InJailTurnCounter; }
	void SetInJailTurnCounter(const int& Value) { InJailTurnCounter = Value >= 0 && Value <= 3 ? Value : 0;} 

private:
	UPROPERTY(ReplicatedUsing=OnRep_CreditBalance)
	int CreditBalance = 1500;
	
	UPROPERTY(ReplicatedUsing=OnRep_DesiredSpaceID, VisibleAnywhere)
	int CurrentSpaceId = 0;
	
	UPROPERTY(ReplicatedUsing=OnRep_IsItMyTurn)
	bool IsItMyTurn = false;

	UPROPERTY(ReplicatedUsing=OnRep_OwnedPropertyList)
	TArray<ABLPPropertySpace*> OwnedPropertyList;

	UPROPERTY(ReplicatedUsing=OnRep_JailTurnCounter)
	int InJailTurnCounter = 0;

	UPROPERTY(ReplicatedUsing=OnRep_GetOutOfJailCounter)
	int GetOutOfJailCounter = 0;

	bool InJail = false;

	UFUNCTION()
	void OnRep_CreditBalance() const;
	
	UFUNCTION()
	void OnRep_DesiredSpaceID() const;

	UFUNCTION()
	void OnRep_IsItMyTurn() const;

	UFUNCTION()
	void OnRep_OwnedPropertyList() const;

	UFUNCTION()
	void OnRep_JailTurnCounter();

	UFUNCTION()
	void OnRep_GetOutOfJailCounter();
	
};
