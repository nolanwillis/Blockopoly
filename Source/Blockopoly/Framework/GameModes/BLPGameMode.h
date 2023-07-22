// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BLPGameMode.generated.h"

class ABLPAvatar;

/**
 * 
 */
UCLASS()
class BLOCKOPOLY_API ABLPGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABLPGameMode();
	void StartGame();
    
protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

private:
	// Pawn References
	TSubclassOf<ABLPAvatar> AvatarClass0;
	TSubclassOf<ABLPAvatar> AvatarClass1;
	TSubclassOf<ABLPAvatar> AvatarClass2;
	TSubclassOf<ABLPAvatar> AvatarClass3;
    	
	FVector InitGameSpawnPoint0 = {-1218, 1200, 38};
	FVector InitGameSpawnPoint1 = {-1218, 1400, 38};
	FVector InitGameSpawnPoint2 = {-1325, 1414, 38};
	FVector InitGameSpawnPoint3 = {-1375, 1330, 38};

	FVector InitLobbySpawnPoint0 = {750, -464, 330};
	FVector InitLobbySpawnPoint1 = {750, -154, 330};
	FVector InitLobbySpawnPoint2 = {750, 164, 330};
	FVector InitLobbySpawnPoint3 = {750, 474, 330};

	void Spawn(const int& BLPPlayerId, APlayerController* NewPlayer) const;
};
