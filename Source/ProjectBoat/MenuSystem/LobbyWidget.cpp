// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "../ProjectBoatGameInstance.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "../ProjectBoatPlayerController.h"

ULobbyWidget::ULobbyWidget(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> WarningBPClass(TEXT("/Game/ProjectBoat/MenuSystem/WBP_Warning"));
	if (!ensure(WarningBPClass.Class != nullptr)) return;
	WarningClass = WarningBPClass.Class;
}

bool ULobbyWidget::Initialize()
{
	bool SuperSucceeded = Super::Initialize();
	if (!SuperSucceeded) return false;

	if (!ensure(LobbySettingsButton != nullptr)) return false;
	LobbySettingsButton->OnClicked.AddDynamic(this, &ULobbyWidget::ShowSettingsMenu);
	if (!ensure(ChangeShipButton != nullptr)) return false;
	ChangeShipButton->OnClicked.AddDynamic(this, &ULobbyWidget::ChangeShip);
	if (!ensure(ReadyButton != nullptr)) return false;
	ReadyButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnReadyButtonPressed);
	if (!ensure(LeaveButton != nullptr)) return false;
	LeaveButton->OnClicked.AddDynamic(this, &ULobbyWidget::LeaveLobby);
	if (!ensure(SettingsPanelSwitcher != nullptr)) return false;

	return true;
}

void ULobbyWidget::ShowWarning(const FString& Message, const FString& ButtonText)
{
	if (!ensure(WarningClass != nullptr)) return;
	WarningWidget = CreateWidget<UWarningWidget>(this, WarningClass);

	WarningWidget->Setup(Message, ButtonText);
}

void ULobbyWidget::ShowSettingsMenu()
{
	APlayerController* LocalPlayerController = GetGameInstance()->GetFirstLocalPlayerController();

	if (LocalPlayerController->HasAuthority())
		SettingsPanelSwitcher->SetActiveWidgetIndex(1);
	else
		SettingsPanelSwitcher->SetActiveWidgetIndex(0);

	SettingsPanelSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void ULobbyWidget::ChangeShip()
{
	TEnumAsByte<ShipTypes> ShipType = GetGameInstance<UProjectBoatGameInstance>()->GetShipType();
	if (ShipType == WarShip)
	{
		ShipType = CargoShip;
		GetGameInstance<UProjectBoatGameInstance>()->SetShipType(ShipType);
	}
	else if (ShipType == CargoShip)
	{
		ShipType = WarShip;
		GetGameInstance<UProjectBoatGameInstance>()->SetShipType(ShipType);
	}
}

void ULobbyWidget::OnReadyButtonPressed()
{
	AProjectBoatPlayerController *PlayerController = Cast<AProjectBoatPlayerController>(GetGameInstance<UProjectBoatGameInstance>()->GetFirstLocalPlayerController());
	PlayerController->SetReady();
	if (ReadyButtonTextSwitcher->GetActiveWidgetIndex() == 0)
		ReadyButtonTextSwitcher->SetActiveWidgetIndex(1);
	else ReadyButtonTextSwitcher->SetActiveWidgetIndex(0);
}

void ULobbyWidget::LeaveLobby()
{
	MenuInterface->Leave();
}