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
DECLARE_DELEGATE(FForfeitSignature);
DECLARE_DELEGATE(FOutOfJailSignature);
DECLARE_DELEGATE(FOnPropertyListChangedSignature);
DECLARE_MULTICAST_DELEGATE(FRefreshUISignature);
DECLARE_MULTICAST_DELEGATE(FPlayerUpIdSignature);
DECLARE_DELEGATE_OneParam(FCanBuySignature, bool Value);
DECLARE_DELEGATE_OneParam(FHasRolledSignature, bool Value);
DECLARE_DELEGATE_OneParam(FInJailSignature, int TurnsLeft);
DECLARE_DELEGATE_OneParam(FJailSkipSignature, const int& JailSkipCounter);
DECLARE_DELEGATE_OneParam(FSaleRequestSignature, const FPropertySaleData& SaleData);
DECLARE_DELEGATE_TwoParams(FSaleResponseSignature, const FPropertySaleData& SaleData, const bool Status);
DECLARE_DELEGATE_ThreeParams(FNotificationSignature, const FString& Type, const FString& Heading, const FString& Description);
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
	
	int GetPlayerUpId() const { return PlayerUpId; }
	void SetPlayerUpId(const int& Value){ PlayerUpId = Value; OnRep_PlayerUpId(); }
	
	int GetBalance() const { return CreditBalance; }
	void AddToBalance(const int& Value) { PreviousCreditBalance = CreditBalance; CreditBalance = CreditBalance+Value; OnRep_CreditBalance();}

	UFUNCTION(BlueprintCallable)
	int GetPreviousBalance() const { return PreviousCreditBalance; }

	int GetCurrentSpaceId() const { return CurrentSpaceId; }
	void SetCurrentSpaceId(const int& Value){ CurrentSpaceId = Value; }

	FSpawnPoint* GetCurrentSpawnPoint() const { return CurrentSpawnPoint; }
	void SetCurrentSpawnPoint(FSpawnPoint* Value){ CurrentSpawnPoint = Value; }
	
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
	void SetHasRolled(const bool& Value){ HasRolled = Value; OnRep_HasRolled(); }
	
	bool GetIsLeaving() const { return IsLeaving; }
	void SetIsLeaving(const bool& Value) { IsLeaving = Value; }

	// Client RPCs
	UFUNCTION(Client, Unreliable, WithValidation, BlueprintCallable)
	void Client_AddNotification(const FString& Type, const FString& Heading, const FString& Description);
	UFUNCTION(Client, Unreliable, WithValidation)
	void Client_RefreshUI();
	UFUNCTION(Client, Unreliable, WithValidation)
	void Client_DisplayWinScreen(const FString& WinnersName);
    UFUNCTION(Client, Unreliable, WithValidation, BlueprintCallable)
	void Client_SimulateMoveLocally(const int NewSpaceId);
	UFUNCTION(Client, Unreliable, WithValidation, BlueprintCallable)
	void Client_AddSaleRequest(const FPropertySaleData& SaleData);
	UFUNCTION(Client, Unreliable, WithValidation, BlueprintCallable)
	void Client_AddSaleResponse(const FPropertySaleData& SaleData, const bool Status);
	
	FPlayerUpIdSignature PlayerUpIdDelegate;
	FOnBalanceChangedSignature OnBalanceChangedDelegate;
	FOnPropertyListChangedSignature OnPropertyListChangedDelegate;
	FSaleRequestSignature SaleRequestDelegate;
	FSaleResponseSignature SaleResponseDelegate;
	FInJailSignature InJailDelegate;
	FOutOfJailSignature OutOfJailDelegate;
	FJailSkipSignature JailSkipDelegate;
	FRefreshUISignature RefreshUIDelegate;
	FCanBuySignature CanBuyDelegate;
	FHasRolledSignature HasRolledDelegate;
	FNotificationSignature NotificationDelegate;
	FForfeitSignature ForfeitDelegate;

private:
	UPROPERTY(Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int BLPPlayerId;  

	UPROPERTY(ReplicatedUsing=OnRep_PlayerUpId)
	int PlayerUpId = -1;
	
	UPROPERTY(ReplicatedUsing=OnRep_CreditBalance)
	int CreditBalance = 1500;

	UPROPERTY(Replicated)
	int PreviousCreditBalance = 1500;
	
	UPROPERTY(Replicated)
	int CurrentSpaceId = 0;

	FSpawnPoint* CurrentSpawnPoint;

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

	// Used for making sure a player state that is in the process of
	// leaving isn't used
	UPROPERTY(Replicated)
	bool IsLeaving = false;

	UPROPERTY()
	ABLPCameraManager* BLPCameraManagerPtr = nullptr;

	TSubclassOf<UUserWidget> WinScreenClass;

	UFUNCTION()
	void OnRep_PlayerUpId() const;
	
	UFUNCTION()
	void OnRep_CreditBalance() const;
	
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
