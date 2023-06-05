// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"

#include "Boat.h"

#include "CustomPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBOAT_API ACustomPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float SpawnScore = 0;

	void CalculateSpawnScore(TArray<ABoat*> FoundBoats);

	void ReserveSpawnPoint();
	

private:
	bool IsAvailable = true;

	void ResetSpawnPoint();
};
