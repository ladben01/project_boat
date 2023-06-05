// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuoyancyComponent.h"
#include "CustomBuoyancyComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBOAT_API UCustomBuoyancyComponent : public UBuoyancyComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = Buoyancy)
		void AddPontoon(FName PontoonCenterSocket, FVector PontoonRelativeLocation, float PontoonRadius) {
            FSphericalPontoon pontoon = FSphericalPontoon();
            pontoon.CenterSocket = PontoonCenterSocket;
            pontoon.RelativeLocation = PontoonRelativeLocation;
            pontoon.Radius = PontoonRadius;
            BuoyancyData.Pontoons.Add(pontoon);
        }
};
