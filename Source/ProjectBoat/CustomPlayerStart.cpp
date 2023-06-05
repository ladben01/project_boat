// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerStart.h"


void ACustomPlayerStart::CalculateSpawnScore(TArray<ABoat*> FoundBoats)
{
	if (IsAvailable)
	{
		SpawnScore = 0;

		float Distance = 9999999;
		for (ABoat* Boat : FoundBoats)
		{
			float NewDistance = FVector::Dist(this->GetActorLocation(), Boat->GetActorLocation());
			if(Distance > NewDistance) Distance = NewDistance;
		}
		
		SpawnScore = Distance;
		/* THIS SPAWN SYSTEM IS GOOD IF WE WANT PLAYERS TO SPAWN KINDA CLOSE (could modify a bit and make a great team-based spawn system)
		for (ABoat* Boat : FoundBoats)
		{
			float Distance = FVector::Dist(this->GetActorLocation(), Boat->GetActorLocation());
			if (Distance < 1000)
				SpawnScore -= 1000000;
			else
				SpawnScore += Distance;
		}*/
	}
	else SpawnScore = -9999999;
}

void ACustomPlayerStart::ReserveSpawnPoint()
{
	IsAvailable = false;
	FTimerHandle UnusedHandle;
	FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &ACustomPlayerStart::ResetSpawnPoint);
	GetWorldTimerManager().SetTimer(UnusedHandle, RespawnDelegate, 5.0f, false);
}

void ACustomPlayerStart::ResetSpawnPoint()
{
	IsAvailable = true;
}
