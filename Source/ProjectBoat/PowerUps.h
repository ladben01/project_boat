// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUps.generated.h"

UENUM()
enum PowerUpTypes
{
	Missile UMETA(DisplayName = "Missile"),
	Torpedo UMETA(DisplayName = "Torpedo"),
	Smoke UMETA(DisplayName = "Smoke"),
	Jump UMETA(DisplayName = "Jump"),
	Dash UMETA(DisplayName = "Dash"),
};

USTRUCT()
struct FPowerUps
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY() TEnumAsByte<PowerUpTypes> PowerUpType = Missile;
	UPROPERTY() float Cooldown = 0;
	UPROPERTY() float CurrentCooldown = 0;
	UPROPERTY() int Count = 0;
	UPROPERTY() bool IsActorNeeded = false;
	UPROPERTY() bool CanSpawn = true;
	UPROPERTY() AActor* PowerUpActor = nullptr;
};
