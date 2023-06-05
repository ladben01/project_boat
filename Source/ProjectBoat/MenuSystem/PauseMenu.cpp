// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"

#include "Components/Button.h"

bool UPauseMenu::Initialize()
{
	bool SuperSucceeded = Super::Initialize();
	if (!SuperSucceeded) return false;

	if (!ensure(BackButton != nullptr)) return false;
	BackButton->OnClicked.AddDynamic(this, &UPauseMenu::BackToGame);
	if (!ensure(MenuButton != nullptr)) return false;
	MenuButton->OnClicked.AddDynamic(this, &UPauseMenu::OpenMainMenu);
	return true;
}

void UPauseMenu::BackToGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Back to the game..."));
	this->Teardown();
}

void UPauseMenu::OpenMainMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("Opening main menu..."));
	MenuInterface->Leave();
}
