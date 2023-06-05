// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "PauseMenu.generated.h"

/**
 *
 */
UCLASS()
class PROJECTBOAT_API UPauseMenu : public UMenuWidget
{
	GENERATED_BODY()


protected:
	virtual bool Initialize();
private:
	UPROPERTY(meta = (BindWidget)) class UButton* BackButton;
	UPROPERTY(meta = (BindWidget)) class UButton* MenuButton;

	UFUNCTION() void BackToGame();
	UFUNCTION() void OpenMainMenu();
};
