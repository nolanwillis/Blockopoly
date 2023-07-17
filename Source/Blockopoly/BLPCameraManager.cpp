// Fill out your copyright notice in the Description page of Project Settings.


#include "BLPCameraManager.h"

#include "Kismet/GameplayStatics.h"

AActor* ABLPCameraManager::GetCamera(const int& Index) const
{
	switch(Index)
	{
	case 0:
		UE_LOG(LogTemp, Warning, TEXT("Camera0 selected!"));
		return Camera0;
	case 1:
		UE_LOG(LogTemp, Warning, TEXT("Camera1 selected!"));
		return Camera1;
	case 2:
		UE_LOG(LogTemp, Warning, TEXT("Camera2 selected!"));
		return Camera2;
	case 3:
		UE_LOG(LogTemp, Warning, TEXT("Camera3 selected!"));
		return Camera3;
	default:
		break;
	}
	UE_LOG(LogTemp, Warning, TEXT("No camera found!"));
	return nullptr;
}


