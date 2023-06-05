// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RacingGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBOAT_API ARacingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ARacingGameModeBase();
	AActor* ChoosePlayerStart_Implementation(AController* Player);
	AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName);
	virtual void HandleSeamlessTravelPlayer(AController*& C);
	virtual void InitSeamlessTravelPlayer(AController* NewController);
	virtual void PostSeamlessTravel();
	UFUNCTION(BlueprintCallable) int32 GetLoadedPlayers() { return LoadedPlayers; };
	UFUNCTION(BlueprintCallable) int32 GetLoadingPlayers() { return LoadingPlayers; };

private:
	int32 LoadingPlayers = 0;
	int32 LoadedPlayers = 0;
};
