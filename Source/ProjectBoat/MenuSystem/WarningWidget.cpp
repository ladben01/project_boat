// Fill out your copyright notice in the Description page of Project Settings.


#include "WarningWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UWarningWidget::Setup(const FString& _Message, const FString& _ButtonText)
{
	this->AddToViewport();
	this->bIsFocusable = true;

	WarningMessage->SetText(FText::FromString(_Message));
	ButtonText->SetText(FText::FromString(_ButtonText));

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

bool UWarningWidget::Initialize()
{
	bool SuperSucceeded = Super::Initialize();
	if (!SuperSucceeded) return false;

	if (!ensure(WarningButton != nullptr)) return false;
	WarningButton->OnClicked.AddDynamic(this, &UWarningWidget::WarningButtonClick);

	return true;
}

void UWarningWidget::Teardown()
{
	this->RemoveFromViewport();
}

void UWarningWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	UWarningWidget::Teardown();

	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UWarningWidget::WarningButtonClick()
{
	Teardown();
}

