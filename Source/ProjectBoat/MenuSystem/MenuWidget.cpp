// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

void UMenuWidget::SetMenuInterface(IMenuInterface* _MenuInterface)
{
	this->MenuInterface = _MenuInterface;
}

void UMenuWidget::Setup()
{
	this->AddToViewport();
	this->bIsFocusable = true;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* LocalPlayerController = World->GetFirstPlayerController();
	if (!ensure(LocalPlayerController != nullptr)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	LocalPlayerController->SetInputMode(InputModeData);
	LocalPlayerController->bShowMouseCursor = true;
}

void UMenuWidget::Teardown()
{
	this->RemoveFromViewport();
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* LocalPlayerController = World->GetFirstPlayerController();
	if (!ensure(LocalPlayerController != nullptr)) return;

	FInputModeGameOnly InputModeData;
	LocalPlayerController->SetInputMode(InputModeData);
	LocalPlayerController->bShowMouseCursor = false;
}

void UMenuWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	UMenuWidget::Teardown();

	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}
