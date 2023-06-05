// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"

void UHUDWidget::SetPackages(TArray<FPackages> Packages)
{
	if (Packages.Num() > 0)
	{
		Package1Info->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		//Package1Img
		Package1Type->SetText(FText::FromString(GetPackageTypeAsString(Packages[0].PackageType)));
		Package1Origin->SetText(FText::FromString("From: " + GetPackageOriginAsString(Packages[0].OriginPort)));
		Package1Destination->SetText(FText::FromString("To: " + GetPackageDestinationAsString(Packages[0].DestinationPort)));
		SetPackageValue(0, Packages[0].CurrentValue);
		SetPackageExplosion(0, Packages[0].ExplodesIn);
		if (Packages.Num() > 1)
		{
			Package2Info->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			//Package2Img
			Package2Type->SetText(FText::FromString(GetPackageTypeAsString(Packages[1].PackageType)));
			Package2Origin->SetText(FText::FromString("From: " + GetPackageOriginAsString(Packages[1].OriginPort)));
			Package2Destination->SetText(FText::FromString("To: " + GetPackageDestinationAsString(Packages[1].DestinationPort)));
			SetPackageValue(1, Packages[1].CurrentValue);
			SetPackageExplosion(1, Packages[1].ExplodesIn);
		}
		else Package2Info->SetVisibility(ESlateVisibility::Hidden);
	}
	else Package1Info->SetVisibility(ESlateVisibility::Hidden);
}

void UHUDWidget::SetPackageValue(int PackageSlot, int32 Value)
{
	if (PackageSlot == 0)
		Package1Value->SetText(FText::FromString("Value: " + FString::FromInt(Value)));
	else
		Package2Value->SetText(FText::FromString("Value: " + FString::FromInt(Value)));
}

void UHUDWidget::SetPackageExplosion(int PackageSlot, float Explosion)
{
	if (PackageSlot == 0)
	{
		if(Explosion != -1)
			Package1Explosion->SetText(FText::FromString("Explodes in: " + FString::FromInt(FMath::Floor(Explosion))));
		else
			Package1Explosion->SetText(FText::FromString(""));
	}
	else
	{
		if (Explosion != -1)
			Package2Explosion->SetText(FText::FromString("Explodes in: " + FString::FromInt(FMath::Floor(Explosion))));
		else
			Package2Explosion->SetText(FText::FromString(""));
	}
}

void UHUDWidget::SetPowerUps(TArray<FPowerUps> PowerUps)
{
	// TODO cleanup this shit
	if (PowerUps.Num() > 0) switch (PowerUps[0].PowerUpType)
	{
	case Missile:
		PrimaryPowerUp->SetText(FText::FromString("Missile " + FString::FromInt(PowerUps[0].Count)));
		break;
	case Torpedo:
		PrimaryPowerUp->SetText(FText::FromString("Torpedo " + FString::FromInt(PowerUps[0].Count)));
		break;
	case Smoke:
		PrimaryPowerUp->SetText(FText::FromString("Smoke " + FString::FromInt(PowerUps[0].Count)));
		break;
	case Jump:
		PrimaryPowerUp->SetText(FText::FromString("Jump " + FString::FromInt(PowerUps[0].Count)));
		break;
	case Dash:
		PrimaryPowerUp->SetText(FText::FromString("Dash " + FString::FromInt(PowerUps[0].Count)));
		break;
	default:
		break;
	}
	else PrimaryPowerUp->SetText(FText::FromString(""));

	if (PowerUps.Num() > 1) switch (PowerUps[1].PowerUpType)
	{
	case Missile:
		SecondaryPowerUp->SetText(FText::FromString("Missile " + FString::FromInt(PowerUps[1].Count)));
		break;
	case Torpedo:
		SecondaryPowerUp->SetText(FText::FromString("Torpedo " + FString::FromInt(PowerUps[1].Count)));
		break;
	case Smoke:
		SecondaryPowerUp->SetText(FText::FromString("Smoke " + FString::FromInt(PowerUps[1].Count)));
		break;
	case Jump:
		SecondaryPowerUp->SetText(FText::FromString("Jump " + FString::FromInt(PowerUps[1].Count)));
		break;
	case Dash:
		SecondaryPowerUp->SetText(FText::FromString("Dash " + FString::FromInt(PowerUps[1].Count)));
		break;
	default:
		break;
	}
	else SecondaryPowerUp->SetText(FText::FromString(""));
	
}