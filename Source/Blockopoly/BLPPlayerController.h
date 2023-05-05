// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BLPGameState.h"
#include "BLPPropertySpace.h"
#include "GameFramework/PlayerController.h"
#include "BLPPlayerController.generated.h"

class ABLPAvatar;
class ABLPGameState; 
class ABLPPlayerState; 
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
	void Server_Move(ABLPAvatar* AvatarPtr, ABLPPlayerState* PlayerStatePtr, const TArray<ABLPSpace*>& LocalSpaceList);

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_RollDice(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr);

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_FinishTurn(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr);

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_ApplySpaceSideEffect(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr);

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_BuyPropertySpace(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr);

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_SellPropertySpace(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr, ABLPPropertySpace* PropertySpacePtr);
};
