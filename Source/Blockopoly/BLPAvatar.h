// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BLPAvatar.generated.h"

UCLASS()
class BLOCKOPOLY_API ABLPAvatar : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABLPAvatar();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void Move();
	void Purchase();
	void FinishTurn();
	// For testing 
	void ListAvailableProperties();
	void Sell();
	void BuyBuilding();
	
	UPROPERTY(EditAnywhere, Category = Components, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* Shape;
};
