// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBoatGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MoviePlayer.h"
#include "ProjectBoatPlayerController.h"

#include "OnlineSessionSettings.h"
#include "Engine/LocalPlayer.h"
#include "ProjectBoatPlayerState.h"
#include "Boat.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/PauseMenu.h"
#include "MenuSystem/LobbyWidget.h"

// TEMP
const static FName SESSION_NAME = TEXT("Test Session");

///-------Konstruktor-------///
// itt beallitjuk a widget osztalyokat, amiket a kesobbiekben hasznalunk
//
UProjectBoatGameInstance::UProjectBoatGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/ProjectBoat/MenuSystem/WBP_MainMenu"));
	if (!ensure(MainMenuBPClass.Class != nullptr)) return;
	MainMenuClass = MainMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> LobbyWidgetBPClass(TEXT("/Game/ProjectBoat/MenuSystem/WBP_LobbyWidget"));
	if (!ensure(LobbyWidgetBPClass.Class != nullptr)) return;
	LobbyWidgetClass = LobbyWidgetBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> PauseMenuBPClass(TEXT("/Game/ProjectBoat/MenuSystem/WBP_PauseMenu"));
	if (!ensure(PauseMenuBPClass.Class != nullptr)) return;
	PauseMenuClass = PauseMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> HUDBPClass(TEXT("/Game/ProjectBoat/HUD/WBP_HUD"));
	if (!ensure(HUDBPClass.Class != nullptr)) return;
	HUDClass = HUDBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> WarningBPClass(TEXT("/Game/ProjectBoat/MenuSystem/WBP_Warning"));
	if (!ensure(WarningBPClass.Class != nullptr)) return;
	WarningClass = WarningBPClass.Class;
}

///-------Init-------///
// Beallitjuk az Online Subsystemet
//
void UProjectBoatGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem: %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Session interface"));
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UProjectBoatGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UProjectBoatGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UProjectBoatGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UProjectBoatGameInstance::OnJoinSessionComplete);
			FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UProjectBoatGameInstance::BeginLoadingScreen);
			FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UProjectBoatGameInstance::EndLoadingScreen);
		}
		else UE_LOG(LogTemp, Warning, TEXT("Could not find session interface"));
	}
	else UE_LOG(LogTemp, Warning, TEXT("Found no subsystem"));

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->OnNetworkFailure().AddUObject(this, &UProjectBoatGameInstance::NetworkError);

	UE_LOG(LogTemp, Warning, TEXT("Found main menu class: %s"), *MainMenuClass->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Found pause menu class: %s"), *PauseMenuClass->GetName());
}

///------Menu betoltese-------///
void UProjectBoatGameInstance::LoadMainMenu()
{
	if (!ensure(MainMenuClass != nullptr)) return;
	Menu = CreateWidget<UMainMenu>(this, MainMenuClass);

	Menu->Setup();
	Menu->SetMenuInterface(this);
}

///------Lobby widget betoltese-------///
void UProjectBoatGameInstance::LoadLobby()
{
	if (!ensure(LobbyWidgetClass != nullptr)) return;
	LobbyWidget = CreateWidget<ULobbyWidget>(this, LobbyWidgetClass);

	LobbyWidget->Setup();
	LobbyWidget->SetMenuInterface(this);
}

///------Pause menu betoltese-------///
void UProjectBoatGameInstance::ChangePauseMenu()
{
	if (!ensure(PauseMenuClass != nullptr)) return;
	PauseMenuWidget = CreateWidget<UPauseMenu>(this, PauseMenuClass);

	PauseMenuWidget->Setup();
	PauseMenuWidget->SetMenuInterface(this);

}

///------HUD betoltese-------///
void UProjectBoatGameInstance::LoadHUD()
{
	if (!ensure(HUDClass != nullptr)) return;
	HUDWidget = CreateWidget<UHUDWidget>(this, HUDClass);
	HUDWidget->AddToViewport();
	HUDWidget->bIsFocusable = true;
}

///------Warning megjelenitese-------///
void UProjectBoatGameInstance::ShowWarning(const FString& Message, const FString& ButtonText)
{
	if (!ensure(WarningClass != nullptr)) return;
	WarningWidget = CreateWidget<UWarningWidget>(this, WarningClass);

	WarningWidget->Setup(Message, ButtonText);
}

void UProjectBoatGameInstance::Host(FString _LobbyType)
{
	if (SessionInterface.IsValid())
	{
		UEngine* Engine = GetEngine();
		if (!ensure(Engine != nullptr)) return;
		Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, TEXT("Hosting a game..."));

		const FString& Message = "Hosting a game...";
		const FString& ButtonText = "Cancel";
		ShowWarning(Message, ButtonText);

		LobbyType = _LobbyType;
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr) // clearing the previous host
		{
			UE_LOG(LogTemp, Warning, TEXT("Found previous session, atempting to destroy it..."));

			IsCreatingSession = true;

			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		ShowWarning("Failed to host: Could not find the session interface.", "Back");
	}
}

void UProjectBoatGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		ShowWarning("Failed to host: Could not create session.", "Back");
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Created a new session successfully!"));

	Menu = nullptr;

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 3, FColor::Green, TEXT("Lobby hosted! Lobby type: " + LobbyType));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	World->ServerTravel("/Game/ProjectBoat/MenuSystem/Map_Lobby?listen");
}

void UProjectBoatGameInstance::StartSession()
{
	SessionInterface->StartSession(SESSION_NAME);

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	ShowWarning("Loading game...", "Ok");
	World->ServerTravel("/Game/smallermap?listen");
}

void UProjectBoatGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not close session"));
		return;
	}
	if (IsCreatingSession)
	{
		IsCreatingSession = false;
		CreateSession();
	}
}

void UProjectBoatGameInstance::OnFindSessionsComplete(bool Success)
{
	if (Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session search finished!"));

		TArray<FString> ServerNames;
		for (FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			ServerNames.Add(SearchResult.GetSessionIdStr());
		}
		Menu->SetServerList(ServerNames);
	}
}

void UProjectBoatGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get connect string..."));
		return;
	}
	Engine->AddOnScreenDebugMessage(0, 3, FColor::Green, FString::Printf(TEXT("Joining server %s..."), *Address));

	APlayerController* LocalPlayerController = GetFirstLocalPlayerController();
	if (!ensure(LocalPlayerController != nullptr)) return;
	LocalPlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UProjectBoatGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	ShowWarning("Connecting to host...", "Cancel");

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
	/**/
}
void UProjectBoatGameInstance::Leave()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, TEXT("Leaving server..."));

	APlayerController* LocalPlayerController = GetFirstLocalPlayerController();
	if (!ensure(LocalPlayerController != nullptr)) return;

	LocalPlayerController->ClientTravel("/Game/ProjectBoat/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
	SessionInterface->DestroySession(SESSION_NAME);
}

void UProjectBoatGameInstance::BeginLoadingScreen(const FString& InMapName)
{
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
}

void UProjectBoatGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
	
}

void UProjectBoatGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Starting session search..."));

		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
	else UE_LOG(LogTemp, Warning, TEXT("Couldn't start session search!"));
}

void UProjectBoatGameInstance::NetworkError(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	Leave();

	ShowWarning("Disconnected from Session.", "Back");
}

void UProjectBoatGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Atempting to create a new session..."));
		FOnlineSessionSettings SessionSettings;

		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}

		if (LobbyType == "Public")
		{
			SessionSettings.NumPublicConnections = 4;
			SessionSettings.bShouldAdvertise = true;
			SessionSettings.bUsesPresence = true;
			SessionSettings.bAllowJoinViaPresenceFriendsOnly = false;
			SessionSettings.bAllowJoinViaPresence = true;
		}
		else if(LobbyType == "Friends Only")
		{
			SessionSettings.NumPrivateConnections = 4;
			SessionSettings.bShouldAdvertise = false;
			SessionSettings.bUsesPresence = true;
			SessionSettings.bAllowJoinViaPresenceFriendsOnly = true;
			SessionSettings.bAllowJoinViaPresence = true;
		}
		else
		{
			SessionSettings.NumPrivateConnections = 4;
			SessionSettings.bShouldAdvertise = false;
			SessionSettings.bUsesPresence = false;
			SessionSettings.bAllowJoinViaPresence = false;
		}

		SessionSettings.bAllowInvites = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.bAllowJoinInProgress = false;
		
		AProjectBoatPlayerState* GameHost = GetFirstLocalPlayerController()->GetPlayerState<AProjectBoatPlayerState>();		
		
		SessionInterface->CreateSession(0, FName(GameHost->GetPlayerName()), SessionSettings);
	}
}

void UProjectBoatGameInstance::SetShipType(ShipTypes Type)
{
	ShipType = Type;

	ABoat* LocalPawn = Cast<ABoat>(GetFirstLocalPlayerController()->AcknowledgedPawn);
	if (LocalPawn != nullptr)
	{
		LocalPawn->ChangeShipType(ShipType);
	}
}