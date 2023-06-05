// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ProjectBoatPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBOAT_API AProjectBoatPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable) void ToggleDebugMode();
	UFUNCTION(BlueprintCallable) bool GetDebugMode();

private:
	bool DebugMode = false;
	
};
