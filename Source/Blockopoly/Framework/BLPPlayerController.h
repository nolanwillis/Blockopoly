// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blockopoly/Items/Spaces/BLPEstatePropertySpace.h"
#include "GameFramework/PlayerController.h"
#include "State/BLPGameState.h"
#include "BLPPlayerController.generated.h"

class ABLPAvatar;
class ABLPEstatePropertySpace;
class ABLPGameState; 
class ABLPPlayerState;
class ABLPPropertySpace;
class ABLPSpace;
class ABLPCameraManager;
class UBLPUWGameMenu;

DECLARE_DELEGATE_OneParam(FOnPlayerJoinSignature, ABLPGameState* GameState);

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API ABLPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UBLPUWGameMenu* GameMenu;

	FOnPlayerJoinSignature OnPlayerJoinDelegate;
	
	ABLPPlayerController();
	
	UFUNCTION(Server, Unreliable, WithValidation, BlueprintCallable)
	void Server_Roll(ABLPAvatar* AvatarPtr, ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr);

	UFUNCTION(Server, Unreliable, WithValidation, BlueprintCallable)
	void Server_ReflectRollInGame(ABLPAvatar* AvatarPtr, ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr);
	
	UFUNCTION(Server, Unreliable, WithValidation, BlueprintCallable)
	void Server_FinishTurn(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr);

	UFUNCTION(Server, Unreliable, WithValidation, BlueprintCallable)
	void Server_BuyPropertySpace(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr);

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_MortgagePropertySpace(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr, const int& SpaceID);
	
	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_UnMortgagePropertySpace(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr, const int& SpaceID);

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_BuyBuilding(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr, const int& SpaceID);

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_ExecuteChanceCard(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr);

	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_ExecuteChestCard(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr);

	// The following funtions must be public for the chance/chest system to work
	void MovePlayer(ABLPAvatar* AvatarPtr, ABLPPlayerState* PlayerStatePtr, const TArray<ABLPSpace*>& SpaceList) const;
	void SendToJail(ABLPPlayerState* PlayerStatePtr, const TArray<ABLPSpace*>& SpaceList) const;
	void ApplySpaceEffect(ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr) const;
	void CheckIfPropertyIsForSale(ABLPPlayerState* PlayerStatePtr, const ABLPGameState* GameStatePtr) const;
	void DrawChanceCard(const ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr) const;
	void DrawChestCard(const ABLPPlayerState* PlayerStatePtr, ABLPGameState* GameStatePtr) const;

protected:
	virtual void BeginPlayingState() override;
	virtual void BeginPlay() override;
	
private:
	void UpdateCanBuild(const ABLPEstatePropertySpace* EstatePropertySpacePtr, const ABLPPlayerState* BLPPlayerStatePtr) const;
	void UpdateBuildings(const ABLPEstatePropertySpace* EstatePropertySpacePtr, const int& BuildingCount) const;
	void UpdateRent(ABLPEstatePropertySpace* EstatePropertySpacePtr, const int& BuildingCount) const;
	void ChargeRent(ABLPPlayerState* PlayerStatePtr, const ABLPGameState* GameStatePtr, const ABLPPropertySpace* EnteredPropertySpace) const;
	
	// Reference to the GameMenu class
	TSubclassOf<UUserWidget> GameMenuClass;

	UPROPERTY()
	ABLPCameraManager* BLPCameraManagerPtr;
	
};
