// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPGameState.h"
#include "BLPProperty.h"

void ABLPGameState::BeginPlay()
{
	for (auto Property : PropertyList)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found property: %s"), *FString(Property->GetName()));
	}
}
