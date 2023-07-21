// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BLPGameState.generated.h"

class ABLPCameraManager;
class ABLPSpace;
class ABLPPropertySpace;
class ABLPPlayerState;

UCLASS()
class BLOCKOPOLY_API ABLPGameState : public AGameState
{
	GENERATED_BODY()
	
	typedef void (ABLPGameState::*BLPGameStateFuncPtr)(ABLPPlayerState* PlayerStatePtr);
	
public:
	TArray<int> GetReadyStatusArray() const { return ReadyStatusArray; }
	void SetReadyStatusArray(const TArray<int>& Value) { ReadyStatusArray = Value; OnRep_ReadyStatusArray(); }
	
	TArray<int> GetForfeitedPlayersArray() const { return ForfeitedPlayersArray; }
	void SetForfeitedPlayersArray(const TArray<int>& Value) { ForfeitedPlayersArray = Value; OnRep_ForfeitedPlayersArray(); }
    	
	ABLPCameraManager* GetCameraManager() const { return BLPCameraManagerPtr; }
	
	ABLPPlayerState* GetOwnerOfProperty(const ABLPPropertySpace* EnteredPropertySpace) const;
	ABLPSpace* GetSpaceFromId(const int& ID) const;
	ABLPPlayerState* GetBLPPlayerStateFromId(const int& ID) const;

	int GetPlayerUpId() const { return GSPlayerUpId; }
	void NextPlayerUp();

	void SetWinnersPlayerId(const int& Value) { WinnersPlayerId = Value; WinnersPlayerIdCallback(); }
	
	TArray<ABLPSpace*> GetSpaceList() const { return SpaceList; }
	void AddToSpaceList(ABLPSpace* Space) { SpaceList.Add(Space); }
	
	TArray<ABLPPropertySpace*> GetAvailablePropertySpaceList () const { return AvailablePropertySpaceList; }
	void AddToAvailablePropertySpaceList(ABLPPropertySpace* Value) { AvailablePropertySpaceList.Add(Value); }
	void RemoveFromAvailablePropertySpaceList(ABLPPropertySpace* Value) { AvailablePropertySpaceList.Remove(Value); }
	
	int GetMaxChanceCards() const { return MaxChanceCards; }
	int GetMaxChestCards() const { return MaxChestCards; }

	int GetCurrentChanceCardIndex() const { return CurrentChanceCardIndex; }
	void SetCurrentChanceCardIndex(const int& Value) { if (Value > MaxChanceCards-1) return;  CurrentChanceCardIndex = Value; }
	int GetCurrentChestCardIndex() const { return CurrentChestCardIndex; }
	void SetCurrentChestCardIndex(const int& Value) { if (Value > MaxChestCards-1) return; CurrentChestCardIndex = Value; }
	
	void ExecuteChanceCard(ABLPPlayerState* PlayerStatePtr);
	void ExecuteChestCard(ABLPPlayerState* PlayerStatePtr);
	
	void AddRollNotificationToUI(const int& Number, const ABLPPlayerState* BLPPlayerStateInPtr);
	void AddForfeitNotificationToUI(const ABLPPlayerState* BLPPlayerStateInPtr);
	void AddLeaveNotificationToUI(const ABLPPlayerState* BLPPlayerStateInPtr);
	void AddCardDrawNotificationToUI(const FString& Type, const FString& Description, const ABLPPlayerState* BLPPlayerStateInPtr);
	

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(ReplicatedUsing=OnRep_ReadyStatusArray)
	TArray<int> ReadyStatusArray = {0, 0, 0, 0};
	
	UPROPERTY(ReplicatedUsing=OnRep_ForfeitedPlayersArray)
	TArray<int> ForfeitedPlayersArray = {};
	
	UPROPERTY()
	ABLPCameraManager* BLPCameraManagerPtr = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General Spaces", meta = (AllowPrivateAccess = true))
	TArray<ABLPSpace*> SpaceList;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere, Category = "Property Spaces", meta = (AllowPrivateAccess = true))
	TArray<ABLPPropertySpace*> AvailablePropertySpaceList;
	
	// Keeps track of which player in the PlayerArray has the current turn
	UPROPERTY()
	int GSPlayerUpId = 0;

	UPROPERTY()
	int WinnersPlayerId = -1;

	UPROPERTY(Replicated)
	int CurrentChanceCardIndex = -1;

	UPROPERTY(Replicated)
	int CurrentChestCardIndex = -1;
	
	// WARNING: Must be changed if the number of cards change
	const int MaxChanceCards = 16;
	const int MaxChestCards = 16;
	
	// Arrays of function pointers, required for community chest and chance systems
	TArray<BLPGameStateFuncPtr> ChanceCards {&ABLPGameState::ChanceCard0, &ABLPGameState::ChanceCard1, &ABLPGameState::ChanceCard2, &ABLPGameState::ChanceCard3,
		                                     &ABLPGameState::ChanceCard4, &ABLPGameState::ChanceCard4, &ABLPGameState::ChanceCard5, &ABLPGameState::ChanceCard6,
		                                     &ABLPGameState::ChanceCard7, &ABLPGameState::ChanceCard8, &ABLPGameState::ChanceCard9, &ABLPGameState::ChanceCard10,
		                                     &ABLPGameState::ChanceCard11, &ABLPGameState::ChanceCard12, &ABLPGameState::ChanceCard13, &ABLPGameState::ChanceCard14};
	TArray<BLPGameStateFuncPtr> ChestCards {&ABLPGameState::ChestCard0, &ABLPGameState::ChestCard1, &ABLPGameState::ChestCard2, &ABLPGameState::ChestCard3,
											 &ABLPGameState::ChestCard4, &ABLPGameState::ChestCard5, &ABLPGameState::ChestCard6, &ABLPGameState::ChestCard7,
											 &ABLPGameState::ChestCard8, &ABLPGameState::ChestCard9, &ABLPGameState::ChestCard10, &ABLPGameState::ChestCard11,
											 &ABLPGameState::ChestCard12, &ABLPGameState::ChestCard13, &ABLPGameState::ChestCard14, &ABLPGameState::ChestCard15};

	// Chance Card Functions
	void ChanceCard0(ABLPPlayerState* PlayerStatePtr);
	void ChanceCard1(ABLPPlayerState* PlayerStatePtr);
	void ChanceCard2(ABLPPlayerState* PlayerStatePtr);
	void ChanceCard3(ABLPPlayerState* PlayerStatePtr);
	void ChanceCard4(ABLPPlayerState* PlayerStatePtr);
	void ChanceCard5(ABLPPlayerState* PlayerStatePtr);
	void ChanceCard6(ABLPPlayerState* PlayerStatePtr);
	void ChanceCard7(ABLPPlayerState* PlayerStatePtr);
	void ChanceCard8(ABLPPlayerState* PlayerStatePtr);
	void ChanceCard9(ABLPPlayerState* PlayerStatePtr);
	void ChanceCard10(ABLPPlayerState* PlayerStatePtr);
	void ChanceCard11(ABLPPlayerState* PlayerStatePtr);
	void ChanceCard12(ABLPPlayerState* PlayerStatePtr);
	void ChanceCard13(ABLPPlayerState* PlayerStatePtr);
	void ChanceCard14(ABLPPlayerState* PlayerStatePtr);

	// Chest Card Functions
	void ChestCard0(ABLPPlayerState* PlayerStatePtr);
	void ChestCard1(ABLPPlayerState* PlayerStatePtr);
	void ChestCard2(ABLPPlayerState* PlayerStatePtr);
	void ChestCard3(ABLPPlayerState* PlayerStatePtr);
	void ChestCard4(ABLPPlayerState* PlayerStatePtr);
	void ChestCard5(ABLPPlayerState* PlayerStatePtr);
	void ChestCard6(ABLPPlayerState* PlayerStatePtr);
	void ChestCard7(ABLPPlayerState* PlayerStatePtr);
	void ChestCard8(ABLPPlayerState* PlayerStatePtr);
	void ChestCard9(ABLPPlayerState* PlayerStatePtr);
	void ChestCard10(ABLPPlayerState* PlayerStatePtr);
	void ChestCard11(ABLPPlayerState* PlayerStatePtr);
	void ChestCard12(ABLPPlayerState* PlayerStatePtr);
	void ChestCard13(ABLPPlayerState* PlayerStatePtr);
	void ChestCard14(ABLPPlayerState* PlayerStatePtr);
	void ChestCard15(ABLPPlayerState* PlayerStatePtr);

	UFUNCTION()
	void OnRep_ForfeitedPlayersArray();
	UFUNCTION()
	void OnRep_ReadyStatusArray();
	UFUNCTION()
	void WinnersPlayerIdCallback();
	
	UFUNCTION()
	void CheckForWinner();
};
