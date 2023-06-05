// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsMenu.h"

bool UOptionsMenu::Initialize()
{
	bool SuperSucceeded = Super::Initialize();
	if (!SuperSucceeded) return false;

	if (!ensure(LowerGraphicsQualityButton != nullptr)) return false;
	LowerGraphicsQualityButton->OnClicked.AddDynamic(this, &UOptionsMenu::LowerGraphicsQuality);
	if (!ensure(HigherGraphicsQualityButton != nullptr)) return false;
	HigherGraphicsQualityButton->OnClicked.AddDynamic(this, &UOptionsMenu::HigherGraphicsQuality);
	if (!ensure(GraphicsQualityLevel != nullptr)) return false;
	SetGraphicsQuality();

	if (!ensure(LowerResolutionScaleButton != nullptr)) return false;
	LowerResolutionScaleButton->OnClicked.AddDynamic(this, &UOptionsMenu::LowerResolutionScale);
	if (!ensure(HigherResolutionScaleButton != nullptr)) return false;
	HigherResolutionScaleButton->OnClicked.AddDynamic(this, &UOptionsMenu::HigherResolutionScale);
	if (!ensure(ResolutionScaleLevel != nullptr)) return false;
	SetResolutionScale();

	if (!ensure(LowerViewDistanceButton != nullptr)) return false;
	LowerViewDistanceButton->OnClicked.AddDynamic(this, &UOptionsMenu::LowerViewDistance);
	if (!ensure(HigherViewDistanceButton != nullptr)) return false;
	HigherViewDistanceButton->OnClicked.AddDynamic(this, &UOptionsMenu::HigherViewDistance);
	if (!ensure(ViewDistanceLevel != nullptr)) return false;
	SetViewDistance();

	if (!ensure(LowerAntiAliasingButton != nullptr)) return false;
	LowerAntiAliasingButton->OnClicked.AddDynamic(this, &UOptionsMenu::LowerAntiAliasing);
	if (!ensure(HigherAntiAliasingButton != nullptr)) return false;
	HigherAntiAliasingButton->OnClicked.AddDynamic(this, &UOptionsMenu::HigherAntiAliasing);
	if (!ensure(AntiAliasingLevel != nullptr)) return false;
	SetAntiAliasing();
	
	if (!ensure(LowerPostProcessingButton != nullptr)) return false;
	LowerPostProcessingButton->OnClicked.AddDynamic(this, &UOptionsMenu::LowerPostProcessing);
	if (!ensure(HigherPostProcessingButton != nullptr)) return false;
	HigherPostProcessingButton->OnClicked.AddDynamic(this, &UOptionsMenu::HigherPostProcessing);
	if (!ensure(PostProcessingLevel != nullptr)) return false;
	SetPostProcessing();
	
	if (!ensure(LowerShadowQualityButton != nullptr)) return false;
	LowerShadowQualityButton->OnClicked.AddDynamic(this, &UOptionsMenu::LowerShadowQuality);
	if (!ensure(HigherShadowQualityButton != nullptr)) return false;
	HigherShadowQualityButton->OnClicked.AddDynamic(this, &UOptionsMenu::HigherShadowQuality);
	if (!ensure(ShadowQualityLevel != nullptr)) return false;
	SetShadowQuality();
	
	if (!ensure(LowerTextureQualityButton != nullptr)) return false;
	LowerTextureQualityButton->OnClicked.AddDynamic(this, &UOptionsMenu::LowerTextureQuality);
	if (!ensure(HigherTextureQualityButton != nullptr)) return false;
	HigherTextureQualityButton->OnClicked.AddDynamic(this, &UOptionsMenu::HigherTextureQuality);
	if (!ensure(TextureQualityLevel != nullptr)) return false;
	SetTextureQuality();
	
	if (!ensure(LowerEffectsQualityButton != nullptr)) return false;
	LowerEffectsQualityButton->OnClicked.AddDynamic(this, &UOptionsMenu::LowerEffectsQuality);
	if (!ensure(HigherEffectsQualityButton != nullptr)) return false;
	HigherEffectsQualityButton->OnClicked.AddDynamic(this, &UOptionsMenu::HigherEffectsQuality);
	if (!ensure(EffectsQualityLevel != nullptr)) return false;
	SetEffectsQuality();
	
	if (!ensure(LowerDetailModeButton != nullptr)) return false;
	LowerDetailModeButton->OnClicked.AddDynamic(this, &UOptionsMenu::LowerDetailMode);
	if (!ensure(HigherDetailModeButton != nullptr)) return false;
	HigherDetailModeButton->OnClicked.AddDynamic(this, &UOptionsMenu::HigherDetailMode);
	if (!ensure(DetailModeLevel != nullptr)) return false;
	SetDetailMode();
	
	if (!ensure(LowerMaterialQualityButton != nullptr)) return false;
	LowerMaterialQualityButton->OnClicked.AddDynamic(this, &UOptionsMenu::LowerMaterialQuality);
	if (!ensure(HigherMaterialQualityButton != nullptr)) return false;
	HigherMaterialQualityButton->OnClicked.AddDynamic(this, &UOptionsMenu::HigherMaterialQuality);
	if (!ensure(MaterialQualityLevel != nullptr)) return false;
	SetMaterialQuality();
	
	if (!ensure(LowerFoliageQualityButton != nullptr)) return false;
	LowerFoliageQualityButton->OnClicked.AddDynamic(this, &UOptionsMenu::LowerFoliageQuality);
	if (!ensure(HigherFoliageQualityButton != nullptr)) return false;
	HigherFoliageQualityButton->OnClicked.AddDynamic(this, &UOptionsMenu::HigherFoliageQuality);
	if (!ensure(FoliageQualityLevel != nullptr)) return false;
	SetFoliageQuality();

	return true;
}

void UOptionsMenu::LowerGraphicsQuality()
{
	GraphicsQuality = LowerValue(4, GraphicsQuality);

	SetGraphicsQuality();
}

void UOptionsMenu::HigherGraphicsQuality()
{
	GraphicsQuality = HigherValue(4, GraphicsQuality);

	SetGraphicsQuality();
}

void UOptionsMenu::LowerResolutionScale()
{
	ResolutionScale = LowerValue(4, ResolutionScale);
	
	SetResolutionScale();
}

void UOptionsMenu::HigherResolutionScale()
{
	ResolutionScale = HigherValue(4, ResolutionScale);

	SetResolutionScale();
}

void UOptionsMenu::LowerViewDistance()
{
	ViewDistance = LowerValue(4, ViewDistance);

	SetViewDistance();
}

void UOptionsMenu::HigherViewDistance()
{
	ViewDistance = HigherValue(4, ViewDistance);

	SetViewDistance();
}

void UOptionsMenu::LowerAntiAliasing()
{
	AntiAliasing = LowerValue(4, AntiAliasing);

	SetAntiAliasing();
}

void UOptionsMenu::HigherAntiAliasing()
{
	AntiAliasing = HigherValue(4, AntiAliasing);

	SetAntiAliasing();
}

void UOptionsMenu::LowerPostProcessing()
{
	PostProcessing = LowerValue(4, PostProcessing);

	SetPostProcessing();
}

void UOptionsMenu::HigherPostProcessing()
{
	PostProcessing = HigherValue(4, PostProcessing);

	SetPostProcessing();
}

void UOptionsMenu::LowerShadowQuality()
{
	ShadowQuality = LowerValue(4, ShadowQuality);

	SetShadowQuality();
}

void UOptionsMenu::HigherShadowQuality()
{
	ShadowQuality = HigherValue(4, ShadowQuality);

	SetShadowQuality();
}

void UOptionsMenu::LowerTextureQuality()
{
	TextureQuality = LowerValue(4, TextureQuality);

	SetTextureQuality();
}

void UOptionsMenu::HigherTextureQuality()
{
	TextureQuality = HigherValue(4, TextureQuality);

	SetTextureQuality();
}

void UOptionsMenu::LowerEffectsQuality()
{
	EffectsQuality = LowerValue(4, EffectsQuality);

	SetEffectsQuality();
}

void UOptionsMenu::HigherEffectsQuality()
{
	EffectsQuality = HigherValue(4, EffectsQuality);

	SetAntiAliasing();
}

void UOptionsMenu::LowerDetailMode()
{
	DetailMode = LowerValue(4, DetailMode);

	SetDetailMode();
}

void UOptionsMenu::HigherDetailMode()
{
	DetailMode = HigherValue(4, DetailMode);

	SetDetailMode();
}

void UOptionsMenu::LowerMaterialQuality()
{
	MaterialQuality = LowerValue(4, MaterialQuality);

	SetMaterialQuality();
}

void UOptionsMenu::HigherMaterialQuality()
{
	MaterialQuality = HigherValue(4, MaterialQuality);

	SetMaterialQuality();
}

void UOptionsMenu::LowerFoliageQuality()
{
	FoliageQuality = LowerValue(4, FoliageQuality);

	SetFoliageQuality();
}

void UOptionsMenu::HigherFoliageQuality()
{
	FoliageQuality = HigherValue(4, FoliageQuality);

	SetFoliageQuality();
}

void UOptionsMenu::SetGraphicsQuality()
{
	GraphicsQualityLevel->SetText(FText::FromString(SetText(GraphicsQuality)));
}

void UOptionsMenu::SetResolutionScale()
{
	FString Command;
	FString Percent;
	switch (ResolutionScale)
	{
	case Low:
		Command = "r.ScreenPercentage 25";
		Percent = "25";
		break;
	case Medium:
		Command = "r.ScreenPercentage 50";
		Percent = "50";
		break;
	case High:
		Command = "r.ScreenPercentage 75";
		Percent = "75";
		break;
	case Epic:
		Command = "r.ScreenPercentage 100";
		Percent = "100";
		break;
	default:
		break;
	}

	ResolutionScaleLevel->SetText(FText::FromString(Percent));
	if(GetOwningPlayer() != nullptr)
		GetOwningPlayer()->ConsoleCommand(Command);
}

void UOptionsMenu::SetViewDistance()
{
	FString Command;
	FString Dist;
	switch (ViewDistance)
	{
	case Low:
		Command = "r.ViewDistanceScale 0.25";
		Dist = "Very Near";
		break;
	case Medium:
		Command = "r.ViewDistanceScale 0.5";
		Dist = "Near";
		break;
	case High:
		Command = "r.ViewDistanceScale 0.75";
		Dist = "Far";
		break;
	case Epic:
		Command = "r.ViewDistanceScale 1.0";
		Dist = "Very Far";
		break;
	default:
		break;
	}

	ViewDistanceLevel->SetText(FText::FromString(Dist));
	if (GetOwningPlayer() != nullptr)
		GetOwningPlayer()->ConsoleCommand(Command);
}

void UOptionsMenu::SetAntiAliasing()
{
	FString Command;
	switch (AntiAliasing)
	{
	case Low:
		Command = "r.PostProcessAAQuality 0";
		break;
	case Medium:
		Command = "r.PostProcessAAQuality 2";
		break;
	case High:
		Command = "r.PostProcessAAQuality 4";
		break;
	case Epic:
		Command = "r.PostProcessAAQuality 6";
		break;
	default:
		break;
	}

	AntiAliasingLevel->SetText(FText::FromString(SetText(AntiAliasing)));
	if (GetOwningPlayer() != nullptr)
		GetOwningPlayer()->ConsoleCommand(Command);
}

void UOptionsMenu::SetPostProcessing()
{
	FString Command;
	switch (PostProcessing)
	{
	case Low:
		Command = "sg.PostProcessQuality 0";
		break;
	case Medium:
		Command = "sg.PostProcessQuality 1";
		break;
	case High:
		Command = "sg.PostProcessQuality 2";
		break;
	case Epic:
		Command = "sg.PostProcessQuality 3";
		break;
	default:
		break;
	}

	PostProcessingLevel->SetText(FText::FromString(SetText(PostProcessing)));
	if (GetOwningPlayer() != nullptr)
		GetOwningPlayer()->ConsoleCommand(Command);
}

void UOptionsMenu::SetShadowQuality()
{
	FString Command;
	switch (ShadowQuality)
	{
	case Low:
		Command = "sg.ShadowQuality 0";
		break;
	case Medium:
		Command = "sg.ShadowQuality 1";
		break;
	case High:
		Command = "sg.ShadowQuality 2";
		break;
	case Epic:
		Command = "sg.ShadowQuality 3";
		break;
	default:
		break;
	}

	ShadowQualityLevel->SetText(FText::FromString(SetText(ShadowQuality)));
	if (GetOwningPlayer() != nullptr)
		GetOwningPlayer()->ConsoleCommand(Command);
}

void UOptionsMenu::SetTextureQuality()
{
	FString Command;
	switch (TextureQuality)
	{
	case Low:
		Command = "sg.TextureQuality 0";
		break;
	case Medium:
		Command = "sg.TextureQuality 1";
		break;
	case High:
		Command = "sg.TextureQuality 2";
		break;
	case Epic:
		Command = "sg.TextureQuality 3";
		break;
	default:
		break;
	}

	TextureQualityLevel->SetText(FText::FromString(SetText(TextureQuality)));
	if (GetOwningPlayer() != nullptr)
		GetOwningPlayer()->ConsoleCommand(Command);
}

void UOptionsMenu::SetEffectsQuality()
{
	FString Command;
	switch (EffectsQuality)
	{
	case Low:
		Command = "sg.EffectsQuality 0";
		break;
	case Medium:
		Command = "sg.EffectsQuality 1";
		break;
	case High:
		Command = "sg.EffectsQuality 2";
		break;
	case Epic:
		Command = "sg.EffectsQuality 3";
		break;
	default:
		break;
	}

	EffectsQualityLevel->SetText(FText::FromString(SetText(EffectsQuality)));
	if (GetOwningPlayer() != nullptr)
		GetOwningPlayer()->ConsoleCommand(Command);
}

void UOptionsMenu::SetDetailMode()
{
	FString Command;
	switch (PostProcessing)
	{
	case Low:
		Command = "sg.PostProcessQuality 0";
		break;
	case Medium:
		Command = "sg.PostProcessQuality 1";
		break;
	case High:
		Command = "sg.PostProcessQuality 2";
		break;
	case Epic:
		Command = "sg.PostProcessQuality 3";
		break;
	default:
		break;
	}

	PostProcessingLevel->SetText(FText::FromString(SetText(PostProcessing)));
	if (GetOwningPlayer() != nullptr)
		GetOwningPlayer()->ConsoleCommand(Command);
}

void UOptionsMenu::SetMaterialQuality()
{
	FString Command;
	switch (PostProcessing)
	{
	case Low:
		Command = "sg.PostProcessQuality 0";
		break;
	case Medium:
		Command = "sg.PostProcessQuality 1";
		break;
	case High:
		Command = "sg.PostProcessQuality 2";
		break;
	case Epic:
		Command = "sg.PostProcessQuality 3";
		break;
	default:
		break;
	}

	PostProcessingLevel->SetText(FText::FromString(SetText(PostProcessing)));
	if (GetOwningPlayer() != nullptr)
		GetOwningPlayer()->ConsoleCommand(Command);
}

void UOptionsMenu::SetFoliageQuality()
{
	FString Command;
	switch (FoliageQuality)
	{
	case Low:
		Command = "FoliageQuality 0";
		break;
	case Medium:
		Command = "FoliageQuality 1";
		break;
	case High:
		Command = "FoliageQuality 2";
		break;
	case Epic:
		Command = "FoliageQuality 3";
		break;
	default:
		break;
	}

	PostProcessingLevel->SetText(FText::FromString(SetText(PostProcessing)));
	if (GetOwningPlayer() != nullptr)
		GetOwningPlayer()->ConsoleCommand(Command);
}

SettingsLevel UOptionsMenu::LowerValue(int32 LevelCount, SettingsLevel CurrentLevel)
{
	if (LevelCount == 3)
	{
		if (CurrentLevel == SettingsLevel::High)
			return SettingsLevel::Medium;
	}
	else if(LevelCount == 4)
	{
		if (CurrentLevel == SettingsLevel::Epic)
			return SettingsLevel::High;
		else if (CurrentLevel == SettingsLevel::High)
			return SettingsLevel::Medium;
	}
	
	return SettingsLevel::Low;
}

SettingsLevel UOptionsMenu::HigherValue(int32 LevelCount, SettingsLevel CurrentLevel)
{
	if (LevelCount == 3)
	{
		if (CurrentLevel == SettingsLevel::Low)
			return SettingsLevel::Medium;
	}
	else if (LevelCount == 4)
	{
		if (CurrentLevel == SettingsLevel::Low)
			return SettingsLevel::Medium;
		else if (CurrentLevel == SettingsLevel::Medium)
			return SettingsLevel::High;
	}
	
	return SettingsLevel::Epic;
}

FString UOptionsMenu::SetText(SettingsLevel CurrentLevel)
{
	switch (CurrentLevel)
	{
	case Low:
		return "Low";
	case Medium:
		return "Medium";
	case High:
		return "High";
	case Epic:
		return "Epic";
	default:
	case Custom:
		return "Custom";
	}
}
