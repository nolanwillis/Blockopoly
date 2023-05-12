// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BLPGameState.generated.h"

class ABLPSpace;
class ABLPPropertySpace;

UCLASS()
class BLOCKOPOLY_API ABLPGameState : public AGameState
{
	GENERATED_BODY()

public:
	TArray<ABLPSpace*> GetSpaceList() const { return SpaceList; }
	void AddToSpaceList(ABLPSpace* Space) { SpaceList.Add(Space); }
	int GetPlayerUpIndex() const { return PlayerUpIndex; }
	void SetPlayerUpIndex(const int& Value) { PlayerUpIndex = Value; }
	TArray<ABLPPropertySpace*> GetAvailablePropertySpaceList () const { return AvailablePropertySpaceList; }
	void AddToAvailablePropertySpaceList(ABLPPropertySpace* Value) { AvailablePropertySpaceList.Add(Value); }
	void RemoveFromAvailablePropertySpaceList(ABLPPropertySpace* Value) { AvailablePropertySpaceList.Remove(Value); }
	
	virtual void BeginPlay() override;

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General Spaces", meta = (AllowPrivateAccess = true))
	TArray<ABLPSpace*> SpaceList;

	UPROPERTY(ReplicatedUsing=OnRep_AvailablePropertySpaces, BlueprintReadWrite, EditAnywhere, Category = "Property Spaces", meta = (AllowPrivateAccess = true))
	TArray<ABLPPropertySpace*> AvailablePropertySpaceList;
	
	// Keeps track of which player in the PlayerArray has the current turn
	int PlayerUpIndex = 0;

	UFUNCTION()
	void OnRep_AvailablePropertySpaces();
};
