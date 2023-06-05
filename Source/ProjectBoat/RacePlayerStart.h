// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "RacePlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBOAT_API ARacePlayerStart : public APlayerStart
{
	GENERATED_BODY()
public:
	bool IsSpawnPointAvailable() { return IsAvailable; };
	void ReserveSpawnPoint() { IsAvailable = false; };

	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool IsStartingPoint = false;
private:
	bool IsAvailable = true;
};
