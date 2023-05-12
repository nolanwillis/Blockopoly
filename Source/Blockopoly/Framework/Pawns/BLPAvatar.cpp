// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPAvatar.h"
#include "../State/BLPGameState.h"
#include "../BLPPlayerController.h"
#include "../State/BLPPlayerState.h"
#include "../../Items/Spaces/BLPSpace.h"
#include "../../Items/Spaces/BLPPropertySpace.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
ABLPAvatar::ABLPAvatar()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Shape = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shape"));
	RootComponent = Shape;

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	} 
}

// Called every frame
void ABLPAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABLPAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("TakeTurn", IE_Pressed, this, &ABLPAvatar::TakeTurn);
	PlayerInputComponent->BindAction("FinishTurn", IE_Pressed, this, &ABLPAvatar::FinishTurn);
	PlayerInputComponent->BindAction("Purchase", IE_Pressed, this, &ABLPAvatar::Purchase);
	PlayerInputComponent->BindAction("ListAvailableProperties", IE_Pressed, this, &ABLPAvatar::ListAvailableProperties);
	PlayerInputComponent->BindAction("Sell", IE_Pressed, this, &ABLPAvatar::Sell);
	PlayerInputComponent->BindAction("BuyBuilding", IE_Pressed, this, &ABLPAvatar::BuyBuilding);
}

// Make Avatar move a random # of spaces by calling server RPCs
void ABLPAvatar::TakeTurn()
{
	ABLPGameState* GameStatePtr = Cast<ABLPGameState>(GetWorld()->GetGameState());
	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(GetOwner());
	ABLPPlayerState* PlayerStatePtr = Cast<ABLPPlayerState>(GetPlayerState());
	
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerState Ptr is null, from Avatar")); return; }
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("GameStatePtr is null, from Avatar")); return; }
	if (!PlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerController Ptr is null, from Avatar")); return; }
	
	const TArray<ABLPSpace*> LocalSpaceList = GameStatePtr->GetSpaceList();
	PlayerControllerPtr->Server_TakeTurn(this, PlayerStatePtr, GameStatePtr);
	
}

void ABLPAvatar::Purchase()
{
	ABLPGameState* GameStatePtr = Cast<ABLPGameState>(GetWorld()->GetGameState());
	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(GetOwner());
	ABLPPlayerState* PlayerStatePtr = Cast<ABLPPlayerState>(GetPlayerState());
	
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerState Ptr is null, from Avatar")); return; }
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("GameStatePtr is null, from Avatar")); return; }
	if (!PlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerController Ptr is null, from Avatar")); return; }

	PlayerControllerPtr->Server_BuyPropertySpace(PlayerStatePtr, GameStatePtr);
}

void ABLPAvatar::FinishTurn()
{
	ABLPGameState* GameStatePtr = Cast<ABLPGameState>(GetWorld()->GetGameState());
	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(GetOwner());
	ABLPPlayerState* PlayerStatePtr = Cast<ABLPPlayerState>(GetPlayerState());
	
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerState Ptr is null, from Avatar")); return; }
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("GameStatePtr is null, from Avatar")); return; }
	if (!PlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerController Ptr is null, from Avatar")); return; }

	// TODO: Attach this to a button in the UI, its here for testing
	PlayerControllerPtr->Server_FinishTurn(PlayerStatePtr, GameStatePtr);
}

void ABLPAvatar::ListAvailableProperties()
{
	const ABLPGameState* GameStatePtr = Cast<ABLPGameState>(GetWorld()->GetGameState());

	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("GameStatePtr is null, from Avatar")); return; }
	
	UE_LOG(LogTemp, Warning, TEXT("Available Property List: "));
	UE_LOG(LogTemp, Warning, TEXT("///////////////////////"));
	TArray<ABLPPropertySpace*> LocalAvailablePropertySpaceList = GameStatePtr->GetAvailablePropertySpaceList();
	for (ABLPPropertySpace* Property : LocalAvailablePropertySpaceList)
	{
		UE_LOG(LogTemp, Warning, TEXT("|Name: %s | Rent: %d|"), *Property->GetName(), Property->GetRent());
	}
	UE_LOG(LogTemp, Warning, TEXT("///////////////////////"));
}

void ABLPAvatar::Sell()
{
	ABLPGameState* GameStatePtr = Cast<ABLPGameState>(GetWorld()->GetGameState());
	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(GetOwner());
	ABLPPlayerState* PlayerStatePtr = Cast<ABLPPlayerState>(GetPlayerState());
	
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("GameStatePtr is null, from Avatar")); return; }
	if (!PlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerController Ptr is null, from Avatar")); return; }
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerState Ptr is null, from Avatar")); return; }

	PlayerControllerPtr->Server_SellPropertySpace(PlayerStatePtr, GameStatePtr, PlayerStatePtr->GetDesiredSpaceID());
}

void ABLPAvatar::BuyBuilding()
{
	ABLPGameState* GameStatePtr = Cast<ABLPGameState>(GetWorld()->GetGameState());
	ABLPPlayerController* PlayerControllerPtr = Cast<ABLPPlayerController>(GetOwner());
	ABLPPlayerState* PlayerStatePtr = Cast<ABLPPlayerState>(GetPlayerState());
	
	if (!GameStatePtr) { UE_LOG(LogTemp, Warning, TEXT("GameStatePtr is null, from Avatar")); return; }
	if (!PlayerControllerPtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerController Ptr is null, from Avatar")); return; }
	if (!PlayerStatePtr) { UE_LOG(LogTemp, Warning, TEXT("PlayerState Ptr is null, from Avatar")); return; }
	
	PlayerControllerPtr->Server_BuyBuilding(PlayerStatePtr, GameStatePtr, PlayerStatePtr->GetDesiredSpaceID());
}





	
	






