// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BLPGameState.h"
#include "GameFramework/PlayerState.h"
#include "BLPPlayerState.generated.h"

struct FSpawnPoint;
class ABLPPropertySpace;
class ABLPPlayerState;
class ABLPCameraManager;
DECLARE_DELEGATE(FOutOfJailSignature);
DECLARE_MULTICAST_DELEGATE(FRefreshUISignature);
DECLARE_DELEGATE_OneParam(FInJailSignature, int TurnsLeft);
DECLARE_DELEGATE_OneParam(FJailSkipSignature, const int& JailSkipCounter);
DECLARE_DELEGATE_OneParam(FCanBuySignature, bool Value);
DECLARE_DELEGATE_OneParam(FHasRolledSignature, bool Value);
DECLARE_DELEGATE_ThreeParams(FNotificationSignature, const FString& Type, const FString& Heading, const FString& Description);
DECLARE_MULTICAST_DELEGATE(FItsMyTurnSignature);
DECLARE_MULTICAST_DELEGATE(FItsNotMyTurnSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBalanceChangedSignature, int NewBalance);

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API ABLPPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABLPPlayerState();

	int GetBLPPlayerId() const { return BLPPlayerId; }
	void SetBLPPlayerId(const int& Value){ BLPPlayerId = Value;}
	
	int GetBalance() const { return CreditBalance; }
	void AddToBalance(const int& Value) { CreditBalance = CreditBalance+Value > 0 ? CreditBalance+Value : 0; OnRep_CreditBalance();}
	
	int GetCurrentSpaceId() const { return CurrentSpaceId; }
	void SetCurrentSpaceId(const int& Value){ CurrentSpaceId = Value; }

	FSpawnPoint* GetCurrentSpawnPoint() const { return CurrentSpawnPoint; }
	void SetCurrentSpawnPoint(FSpawnPoint* Value){ CurrentSpawnPoint = Value; }
	
	bool GetIsItMyTurn() const { return IsItMyTurn; }
	void SetIsItMyTurn(const bool& Value) { IsItMyTurn = Value; OnRep_IsItMyTurn();}
	
	TArray<ABLPPropertySpace*> GetOwnedPropertyList() const { return OwnedPropertyList; }
	void AddToOwnedPropertyList(ABLPPropertySpace* Value) { OwnedPropertyList.Add(Value); OnRep_OwnedPropertyList();}
	void RemoveFromOwnedPropertyList(ABLPPropertySpace* Value) { OwnedPropertyList.Remove(Value); OnRep_OwnedPropertyList();}
	
	int GetJailCounter() const { return JailCounter;}
	void SetJailCounter(const int& Value) { JailCounter = Value >= 0 && Value <= 3 ? Value : 0; OnRep_JailCounter();}
	
	int GetJailSkipCounter() const { return JailSkipCounter; }
	void SetJailSkipCounter(const int& Value) { JailSkipCounter = Value; OnRep_JailSkipCounter();}

	void SetPlayerCount(const int& Value) { PlayerCount = Value; OnRep_PlayerCount();}

	bool GetCanBuyCurrentProperty() const { return CanBuyCurrentProperty; }
	void SetCanBuyCurrentProperty(const bool& Value) { CanBuyCurrentProperty = Value; OnRep_CanBuyCurrentProperty(); }

	bool GetHasRolled() const { return HasRolled; }
	void SetHasRolled(const bool& Value) { HasRolled = Value; OnRep_HasRolled(); }
	
	UFUNCTION(Client, Unreliable, WithValidation, BlueprintCallable)
	void Client_AddNotification(const FString& Type, const FString& Heading, const FString& Description);

	UFUNCTION(Client, Unreliable, WithValidation)
	void Client_RefreshUI();
    
    UFUNCTION(Client, Unreliable, WithValidation, BlueprintCallable)
	void Client_SimulateMoveLocally(const int NewSpaceId);

	FItsMyTurnSignature ItsMyTurnDelegate;
	FItsNotMyTurnSignature ItsNotMyTurnDelegate;
	FOnBalanceChangedSignature OnBalanceChangedDelegate;
	FInJailSignature InJailDelegate;
	FOutOfJailSignature OutOfJailDelegate;
	FJailSkipSignature JailSkipDelegate;
	FRefreshUISignature RefreshUIDelegate;
	FCanBuySignature CanBuyDelegate;
	FHasRolledSignature HasRolledDelegate;
	FNotificationSignature NotificationDelegate;


private:
	UPROPERTY(Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int BLPPlayerId;  
	
	UPROPERTY(ReplicatedUsing=OnRep_CreditBalance)
	int CreditBalance = 1000000;
	
	UPROPERTY(Replicated)
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

	// Local player count, used to keep player list up to date
	UPROPERTY(ReplicatedUsing=OnRep_PlayerCount)
	int PlayerCount;

	UPROPERTY(ReplicatedUsing=OnRep_CanBuyCurrentProperty)
	bool CanBuyCurrentProperty = false;

	// Keeps track of if the player has rolled during their turn
	UPROPERTY(ReplicatedUsing=OnRep_HasRolled)
	bool HasRolled = false;

	UPROPERTY()
	ABLPCameraManager* BLPCameraManagerPtr = nullptr;
	
	UFUNCTION()
	void OnRep_CreditBalance() const;
	
	UFUNCTION()
	void OnRep_IsItMyTurn() const;

	UFUNCTION()
	void OnRep_OwnedPropertyList() const;

	UFUNCTION()
	void OnRep_JailCounter();

	UFUNCTION()
	void OnRep_JailSkipCounter();

	UFUNCTION()
	void OnRep_PlayerCount();

	UFUNCTION()
	void OnRep_CanBuyCurrentProperty();

	UFUNCTION()
	void OnRep_HasRolled();
};
