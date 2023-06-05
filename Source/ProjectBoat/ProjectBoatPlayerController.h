// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectBoatPlayerController.generated.h"



UCLASS()
class PROJECTBOAT_API AProjectBoatPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AProjectBoatPlayerController(const FObjectInitializer& ObjectIn);
	UFUNCTION(Server, Reliable) void SetReady();
	UFUNCTION(BlueprintCallable) bool GetReady();

	UFUNCTION(Server, Reliable) void SetLoadingComplete();
	UFUNCTION(BlueprintCallable) bool GetLoadingComplete();

private:
	bool IsReady = false;
	bool LoadingComplete = false;
	
};
