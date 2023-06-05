// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "WarningWidget.h"
#include "../BoatType.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBOAT_API ULobbyWidget : public UMenuWidget
{
	GENERATED_BODY()
public:
	ULobbyWidget(const FObjectInitializer& ObjectInitializer);
protected:
	virtual bool Initialize();
private:
	UPROPERTY(meta = (BindWidget)) class UButton* LobbySettingsButton;
	UPROPERTY(meta = (BindWidget)) class UButton* ChangeShipButton;
	UPROPERTY(meta = (BindWidget)) class UButton* ReadyButton;
	UPROPERTY(meta = (BindWidget)) class UButton* LeaveButton;
	UPROPERTY(meta = (BindWidget)) class UWidgetSwitcher* SettingsPanelSwitcher;
	UPROPERTY(meta = (BindWidget)) class UWidgetSwitcher* ReadyButtonTextSwitcher;

	UFUNCTION() void ShowWarning(const FString& Message, const FString& ButtonText);
	UFUNCTION() void ShowSettingsMenu();
	UFUNCTION() void ChangeShip();
	UFUNCTION() void OnReadyButtonPressed();
	UFUNCTION() void LeaveLobby();

	UWarningWidget* WarningWidget;
	TSubclassOf<class UUserWidget> WarningClass;
	
};
