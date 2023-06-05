// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "OptionsMenu.generated.h"

UENUM()
enum SettingsLevel
{
	Low UMETA(DisplayName = "Low"),
	Medium UMETA(DisplayName = "Medium"),
	High UMETA(DisplayName = "High"),
	Epic UMETA(DisplayName = "Epic"),
	Custom UMETA(DisplayName = "Custom"),
};

UCLASS()
class PROJECTBOAT_API UOptionsMenu : public UMenuWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize();
private:
	UPROPERTY(meta = (BindWidget)) class UButton* LowerGraphicsQualityButton;
	UPROPERTY(meta = (BindWidget)) class UButton* HigherGraphicsQualityButton;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* GraphicsQualityLevel;

	UPROPERTY(meta = (BindWidget)) class UButton* LowerResolutionScaleButton;
	UPROPERTY(meta = (BindWidget)) class UButton* HigherResolutionScaleButton;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ResolutionScaleLevel;

	UPROPERTY(meta = (BindWidget)) class UButton* LowerViewDistanceButton;
	UPROPERTY(meta = (BindWidget)) class UButton* HigherViewDistanceButton;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ViewDistanceLevel;

	UPROPERTY(meta = (BindWidget)) class UButton* LowerAntiAliasingButton;
	UPROPERTY(meta = (BindWidget)) class UButton* HigherAntiAliasingButton;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* AntiAliasingLevel;
	
	UPROPERTY(meta = (BindWidget)) class UButton* LowerPostProcessingButton;
	UPROPERTY(meta = (BindWidget)) class UButton* HigherPostProcessingButton;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* PostProcessingLevel;
	
	UPROPERTY(meta = (BindWidget)) class UButton* LowerShadowQualityButton;
	UPROPERTY(meta = (BindWidget)) class UButton* HigherShadowQualityButton;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ShadowQualityLevel;
	
	UPROPERTY(meta = (BindWidget)) class UButton* LowerTextureQualityButton;
	UPROPERTY(meta = (BindWidget)) class UButton* HigherTextureQualityButton;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* TextureQualityLevel;
	
	UPROPERTY(meta = (BindWidget)) class UButton* LowerEffectsQualityButton;
	UPROPERTY(meta = (BindWidget)) class UButton* HigherEffectsQualityButton;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* EffectsQualityLevel;
	
	UPROPERTY(meta = (BindWidget)) class UButton* LowerDetailModeButton;
	UPROPERTY(meta = (BindWidget)) class UButton* HigherDetailModeButton;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* DetailModeLevel;
	
	UPROPERTY(meta = (BindWidget)) class UButton* LowerMaterialQualityButton;
	UPROPERTY(meta = (BindWidget)) class UButton* HigherMaterialQualityButton;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* MaterialQualityLevel;
	
	UPROPERTY(meta = (BindWidget)) class UButton* LowerFoliageQualityButton;
	UPROPERTY(meta = (BindWidget)) class UButton* HigherFoliageQualityButton;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* FoliageQualityLevel;

	TEnumAsByte <SettingsLevel> GraphicsQuality = SettingsLevel::Medium;
	TEnumAsByte <SettingsLevel> ResolutionScale = SettingsLevel::Epic;
	TEnumAsByte <SettingsLevel> ViewDistance = SettingsLevel::Medium;
	TEnumAsByte <SettingsLevel> AntiAliasing = SettingsLevel::Medium;
	TEnumAsByte <SettingsLevel> PostProcessing = SettingsLevel::Medium;
	TEnumAsByte <SettingsLevel> ShadowQuality = SettingsLevel::Medium;
	TEnumAsByte <SettingsLevel> TextureQuality = SettingsLevel::Medium;
	TEnumAsByte <SettingsLevel> EffectsQuality = SettingsLevel::Medium;
	TEnumAsByte <SettingsLevel> DetailMode = SettingsLevel::Medium;
	TEnumAsByte <SettingsLevel> MaterialQuality = SettingsLevel::Medium;
	TEnumAsByte <SettingsLevel> FoliageQuality = SettingsLevel::Medium;

	UFUNCTION() void LowerGraphicsQuality();
	UFUNCTION() void HigherGraphicsQuality();
	UFUNCTION() void LowerResolutionScale();
	UFUNCTION() void HigherResolutionScale();
	UFUNCTION() void LowerViewDistance();
	UFUNCTION() void HigherViewDistance();
	UFUNCTION() void LowerAntiAliasing();
	UFUNCTION() void HigherAntiAliasing();
	UFUNCTION() void LowerPostProcessing();
	UFUNCTION() void HigherPostProcessing();
	UFUNCTION() void LowerShadowQuality();
	UFUNCTION() void HigherShadowQuality();
	UFUNCTION() void LowerTextureQuality();
	UFUNCTION() void HigherTextureQuality();
	UFUNCTION() void LowerEffectsQuality();
	UFUNCTION() void HigherEffectsQuality();
	UFUNCTION() void LowerDetailMode();
	UFUNCTION() void HigherDetailMode();
	UFUNCTION() void LowerMaterialQuality();
	UFUNCTION() void HigherMaterialQuality();
	UFUNCTION() void LowerFoliageQuality();
	UFUNCTION() void HigherFoliageQuality();

	void SetGraphicsQuality();
	void SetResolutionScale();
	void SetViewDistance();
	void SetAntiAliasing();
	void SetPostProcessing();
	void SetShadowQuality();
	void SetTextureQuality();
	void SetEffectsQuality();
	void SetDetailMode();
	void SetMaterialQuality();
	void SetFoliageQuality();

	SettingsLevel LowerValue(int32 LevelCount, SettingsLevel CurrentLevel);
	SettingsLevel HigherValue(int32 LevelCount, SettingsLevel CurrentLevel);
	FString SetText(SettingsLevel CurrentLevel);
};
