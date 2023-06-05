// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum ShipTypes
{
	CargoShip UMETA(DisplayName = "CargoShip"),
	WarShip UMETA(DisplayName = "WarShip"),
};

FString GetShipTypeAsString(ShipTypes ShipType);
