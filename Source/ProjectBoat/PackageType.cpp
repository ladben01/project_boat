// Fill out your copyright notice in the Description page of Project Settings.


#include "PackageType.h"

FString GetPackageTypeAsString(PackageTypes Type)
{
	switch (Type)
	{
	default:
	case Regular:
		return "Regular Goods";
	case Bomb:
		return "Explosives";
	case Fragile:
		return "Fragile";
	}
}

FString GetPackageOriginAsString(APort *Port)
{
	if (Port == nullptr) return "null";
	else return Port->GetPortName();
}

FString GetPackageDestinationAsString(APort* Port)
{
	if (Port == nullptr) return "Any Port";
	else return Port->GetPortName();
}