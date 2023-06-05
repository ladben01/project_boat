// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

/**
 *
 */
UCLASS()
class PROJECTBOAT_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()
public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);
	void SetServerList(TArray<FString> ServerNames);

	void SelectServerIndex(uint32 ServerIndex);
protected:
	virtual bool Initialize();
private:
	UPROPERTY(meta = (BindWidget)) class UButton* HostButton;
	UPROPERTY(meta = (BindWidget)) class UButton* JoinButton;
	UPROPERTY(meta = (BindWidget)) class UButton* OptionsButton;
	UPROPERTY(meta = (BindWidget)) class UButton* QuitButton;
	UPROPERTY(meta = (BindWidget)) class UWidgetSwitcher* MenuSwitcher;
	UPROPERTY(meta = (BindWidget)) class UButton* JoinBackButton;
	UPROPERTY(meta = (BindWidget)) class UButton* RefreshButton;
	UPROPERTY(meta = (BindWidget)) class UButton* ConnectButton;
	UPROPERTY(meta = (BindWidget)) class UButton* HostConfirmButton;
	UPROPERTY(meta = (BindWidget)) class UButton* HostBackButton;
	UPROPERTY(meta = (BindWidget)) class UButton* OptionsBackButton;
	UPROPERTY(meta = (BindWidget)) class UWidget* MainMenu;
	UPROPERTY(meta = (BindWidget)) class UWidget* HostMenu;
	UPROPERTY(meta = (BindWidget)) class UWidget* JoinMenu;
	UPROPERTY(meta = (BindWidget)) class UWidget* OptionsMenu;
	UPROPERTY(meta = (BindWidget)) class UPanelWidget* ServerList;
	UPROPERTY(meta = (BindWidget)) class UComboBoxString* LobbyType;

	TSubclassOf<class UUserWidget> ServerRowClass;

	UFUNCTION() void HostServer();
	UFUNCTION() void OpenJoinMenu();
	UFUNCTION() void OpenHostMenu();
	UFUNCTION() void OpenOptionsMenu();
	UFUNCTION() void JoinServer();
	UFUNCTION() void RefreshList();
	UFUNCTION() void OpenMainMenu();
	UFUNCTION() void QuitFromGame();

	TOptional<uint32> SelectedServerIndex;
};
