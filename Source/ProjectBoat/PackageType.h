// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Port.h"
#include "PackageType.generated.h"

UENUM()
enum PackageTypes
{
	Regular UMETA(DisplayName = "Regular"),
	Bomb UMETA(DisplayName = "Bomb"),
	Fragile UMETA(DisplayName = "Fragile"),
};

USTRUCT()
struct FPackages
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY() APort* OriginPort = nullptr;
	UPROPERTY() APort* DestinationPort = nullptr;

	UPROPERTY() TEnumAsByte<PackageTypes> PackageType = Regular;
	UPROPERTY() int32 CurrentValue = 0;
	UPROPERTY() float ValueDecreaseTime = -1;
	UPROPERTY() float ValueDecreasesIn = -1;
	UPROPERTY() float ExplodesIn = -1;

	UPROPERTY() FVector Location;
	UPROPERTY() AActor* Actor;
};

FString GetPackageTypeAsString(PackageTypes Type);
FString GetPackageOriginAsString(APort* Port);
FString GetPackageDestinationAsString(APort* Port);
