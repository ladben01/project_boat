// Fill out your copyright notice in the Description page of Project Settings.


#include "BoatType.h"

FString GetShipTypeAsString(ShipTypes ShipType)
{
	switch (ShipType)
	{
	default:
	case CargoShip:
		return "Cargoship";
	case WarShip:
		return "Warship";
	}
}
