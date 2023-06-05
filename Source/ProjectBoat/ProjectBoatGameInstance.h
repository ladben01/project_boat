// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "MenuSystem/MenuWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/WarningWidget.h"
#include "HUDWidget.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "BoatType.h"
#include "ProjectBoatGameInstance.generated.h"

UCLASS()
class PROJECTBOAT_API UProjectBoatGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
		UProjectBoatGameInstance(const FObjectInitializer& ObjectInitializer);
	void Init();

public:
	UFUNCTION(BlueprintCallable) void LoadMainMenu();
	UFUNCTION(BlueprintCallable) void LoadLobby();
	UFUNCTION(BlueprintCallable) void StartSession();
	UFUNCTION(BlueprintCallable) void ChangePauseMenu();
	UFUNCTION(BlueprintCallable) void LoadHUD();
	UFUNCTION(BlueprintCallable) void ShowWarning(const FString& Message, const FString& ButtonText);

	UFUNCTION(Exec) void Host(FString _LobbyType);
	UFUNCTION(Exec) void Join(uint32 Index) override;
	UFUNCTION() virtual void BeginLoadingScreen(const FString& MapName);
	UFUNCTION() virtual void EndLoadingScreen(UWorld* InLoadedWorld);
	virtual void Leave() override;
	virtual void RefreshServerList() override;

	UFUNCTION(BlueprintCallable) TEnumAsByte<ShipTypes> GetShipType() { return ShipType; };
	UFUNCTION(BlueprintCallable) void SetShipType(ShipTypes Type);

	UFUNCTION(BlueprintCallable) UHUDWidget* GetHUDWidget() { return HUDWidget; };

	void NetworkError(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
private:
	TSubclassOf<class UUserWidget> MainMenuClass;
	TSubclassOf<class UUserWidget> LobbyWidgetClass;
	TSubclassOf<class UUserWidget> PauseMenuClass;
	TSubclassOf<class UUserWidget> HUDClass;
	TSubclassOf<class UUserWidget> WarningClass;
	UMenuWidget* PauseMenuWidget;
	UMenuWidget* LobbyWidget;
	UMainMenu* Menu;
	UHUDWidget* HUDWidget;
	UWarningWidget* WarningWidget;
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	FString LobbyType = "Public";

	bool IsCreatingSession = false;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();


	TEnumAsByte<ShipTypes> ShipType = WarShip;
};

