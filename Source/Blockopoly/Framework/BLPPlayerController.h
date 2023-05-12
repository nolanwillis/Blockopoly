// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "State/BLPGameState.h"
#include "State/BLPPlayerState.h"
#include "BLPPlayerController.generated.h"

class ABLPAvatar;
class ABLPEstatePropertySpace;
class ABLPGameState; 
class ABLPPlayerState;
class ABLPPropertySpace;
class ABLPSpace;

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API ABLPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_TakeTurn(ABLPAvatar* AvatarPtr, ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr);

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_FinishTurn(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr);

	// UFUNCTION(Server, Unreliable, WithValidation)
	// void Server_ApplySpaceSideEffect(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr);

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_BuyPropertySpace(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr);

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_SellPropertySpace(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr, const int& SpaceID);

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_BuyBuilding(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr, const int& SpaceID);
	

private:
	void RollDice(ABLPPlayerState* PlayerStatePtr, const ABLPGameState* GameStatePtr) const;
	void MovePlayer(ABLPAvatar* AvatarPtr, const ABLPPlayerState* PlayerStatePtr, TArray<ABLPSpace*> SpaceList) const;
	void ApplySpaceSideEffect(ABLPPlayerState* PlayerStatePtr, const ABLPGameState* GameStatePtr) const;
	void PropertySpaceSideEffect(ABLPPlayerState* PlayerStatePtr, const ABLPGameState* GameStatePtr, const ABLPPropertySpace* EnteredPropertySpace) const;
	void UpdateBuildings(const ABLPEstatePropertySpace* EstatePropertySpacePtr, const int& BuildingCount);
	
};
