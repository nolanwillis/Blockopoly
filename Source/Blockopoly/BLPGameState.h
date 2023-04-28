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
	TArray<ABLPSpace*> GetSpaceList() { return SpaceList; }
	void AddToSpaceList(ABLPSpace* Space) { SpaceList.Add(Space); }

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property Management", meta = (AllowPrivateAccess = true))
	TArray<ABLPSpace*> SpaceList;
};
