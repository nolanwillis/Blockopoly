// Fill out your copyright notice in the Description page of Project Settings.


#include "./BLPJailSpace.h"

FSpawnPoint* ABLPJailSpace::GetOpenJailCell()
{
	// Find the first open jail cell and return it. We don't need a case for when all are taken because there
	// will always be enough jail cells.
	for (const auto Cell : JailCells)
	{
		if (!Cell->Taken)
		{
			Cell->Taken = true;
			return Cell;
		}
	}
	return nullptr;
}

void ABLPJailSpace::BeginPlay()
{
	Super::BeginPlay();

	JailCell0.Index = 4;
	JailCell0.Taken = false;
	JailCell0.Index = 5;
	JailCell0.Taken = false;
	JailCell0.Index = 6;
	JailCell0.Taken = false;
	JailCell0.Index = 7;
	JailCell0.Taken = false;
}
