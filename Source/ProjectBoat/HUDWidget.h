// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Blueprint/UserWidget.h"
#include "PackageType.h"
#include "PowerUps.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBOAT_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetHintText(FString Text) { Hint->SetText(FText::FromString(Text)); };
	void SetErrorText(FString Text) { Error->SetText(FText::FromString(Text)); };
	void SetBoost(float Max, float Current) { BoostBar->SetPercent(Current / Max); BoostText->SetText(FText::FromString(FString::FromInt(FMath::Floor(Current)))); };
	void SetPackages(TArray<FPackages> Packages);
	void SetPackageValue(int PackageSlot, int32 Value);
	void SetPackageExplosion(int PackageSlot, float Explosion);
	void SetPowerUps(TArray<FPowerUps> PowerUps);
private:
	UPROPERTY(meta = (BindWidget)) UTextBlock* GameTimer;

	UPROPERTY(meta = (BindWidget)) UProgressBar* BoostBar;
	UPROPERTY(meta = (BindWidget)) UTextBlock* BoostText;

	UPROPERTY(meta = (BindWidget)) UTextBlock* FirstPlace;
	UPROPERTY(meta = (BindWidget)) UTextBlock* SecondPlace;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ThirdPlace;
	UPROPERTY(meta = (BindWidget)) UTextBlock* FourthPlace;

	UPROPERTY(meta = (BindWidget)) UHorizontalBox* Package1Info;
	UPROPERTY(meta = (BindWidget)) UImage* Package1Img;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Package1Type;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Package1Origin;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Package1Destination;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Package1Value;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Package1Explosion;
	
	UPROPERTY(meta = (BindWidget)) UHorizontalBox* Package2Info;
	UPROPERTY(meta = (BindWidget)) UImage* Package2Img;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Package2Type;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Package2Origin;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Package2Destination;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Package2Value;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Package2Explosion;

	UPROPERTY(meta = (BindWidget)) UTextBlock* PrimaryPowerUp;
	UPROPERTY(meta = (BindWidget)) UTextBlock* SecondaryPowerUp;

	UPROPERTY(meta = (BindWidget)) UTextBlock* Hint;
	UPROPERTY(meta = (BindWidget)) UTextBlock* Error;
};
