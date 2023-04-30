// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BLPGameState.generated.h"

class ABLPSpace;

UCLASS()
class BLOCKOPOLY_API ABLPGameState : public AGameState
{
	GENERATED_BODY()

public:
	TArray<ABLPSpace*> GetSpaceList() const { return SpaceList; }
	void AddToSpaceList(ABLPSpace* Space) { SpaceList.Add(Space); }
	int GetPlayerUpIndex() const { return PlayerUpIndex; }
	void SetPlayerUpIndex(const int& Value) { PlayerUpIndex = Value; }
	
	virtual void BeginPlay() override;

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property Management", meta = (AllowPrivateAccess = true))
	TArray<ABLPSpace*> SpaceList;

	// Keeps track of which player in the PlayerArray has the current turn
	int PlayerUpIndex = 0;
};
