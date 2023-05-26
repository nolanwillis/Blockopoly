// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BLPPlayerState.generated.h"

struct FSpawnPoint;
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

	FSpawnPoint* GetCurrentSpawnPoint() const { return CurrentSpawnPoint; }
	void SetCurrentSpawnPoint(FSpawnPoint* Value){ CurrentSpawnPoint = Value; }
	
	bool GetIsItMyTurn() const { return IsItMyTurn; }
	void SetIsItMyTurn(const bool& Value) { IsItMyTurn = Value; }
	
	TArray<ABLPPropertySpace*> GetOwnedPropertyList() const { return OwnedPropertyList; }
	void AddToOwnedPropertyList(ABLPPropertySpace* Value) { OwnedPropertyList.Add(Value); }
	void RemoveFromOwnedPropertyList(ABLPPropertySpace* Value) { OwnedPropertyList.Remove(Value); }
	
	int GetJailCounter() const { return JailCounter; }
	void SetJailCounter(const int& Value) { JailCounter = Value >= 0 && Value <= 3 ? Value : 0; }
	
	int GetJailSkipCounter() const { return JailSkipCounter; }
	void SetJailSkipCounter(const int& Value) { JailSkipCounter = Value; }

private:
	UPROPERTY(ReplicatedUsing=OnRep_CreditBalance)
	int CreditBalance = 1500;
	
	UPROPERTY(ReplicatedUsing=OnRep_DesiredSpaceID, VisibleAnywhere)
	int CurrentSpaceId = 0;

	FSpawnPoint* CurrentSpawnPoint;
	
	UPROPERTY(ReplicatedUsing=OnRep_IsItMyTurn)
	bool IsItMyTurn = false;

	UPROPERTY(ReplicatedUsing=OnRep_OwnedPropertyList)
	TArray<ABLPPropertySpace*> OwnedPropertyList;

	// How many turns the player must be in jail for
	UPROPERTY(ReplicatedUsing=OnRep_JailCounter)
	int JailCounter = 0;

	// Get out jail free card counter
	UPROPERTY(ReplicatedUsing=OnRep_JailSkipCounter)
	int JailSkipCounter = 0;

	bool InJail = false;

	UFUNCTION()
	void OnRep_CreditBalance() const;
	
	UFUNCTION()
	void OnRep_DesiredSpaceID();

	UFUNCTION()
	void OnRep_IsItMyTurn() const;

	UFUNCTION()
	void OnRep_OwnedPropertyList() const;

	UFUNCTION()
	void OnRep_JailCounter();

	UFUNCTION()
	void OnRep_JailSkipCounter();
};
