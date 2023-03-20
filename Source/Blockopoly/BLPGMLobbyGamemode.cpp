// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPGMLobbyGamemode.h"
#include "BLPGameInstance.h"

void ABLPGMLobbyGamemode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++PlayerCount;

	if (PlayerCount >= 2)
	{
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ABLPGMLobbyGamemode::StartGame, 10);
	}
}

void ABLPGMLobbyGamemode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--PlayerCount;
}

void ABLPGMLobbyGamemode::StartGame()
{
	auto GameInstance = Cast<UBLPGameInstance>(GetGameInstance());
	if (!GameInstance) return;
	GameInstance->StartSession();
	UWorld* World = GetWorld();
	if (!World) return;
	// Enables seamless travel (connects players transition map first, then to target map)
	bUseSeamlessTravel = true;
	// Tells all player controllers connected to travel to a different map as a listen server 
	World->ServerTravel("/Game/Maps/LVL_Game");
}

