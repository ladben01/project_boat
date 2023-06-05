// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "ServerRow.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"


UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/ProjectBoat/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;
	ServerRowClass = ServerRowBPClass.Class;
}

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	ServerList->ClearChildren();

	uint32 Index = 0;
	if (ServerNames.Num() == 0)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->ServerName->SetText(FText::FromString("Could not find any servers!"));
		Row->Setup(this, -1);

		ServerList->AddChild(Row);
	}
	else for (const FString& ServerName : ServerNames)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->ServerName->SetText(FText::FromString(ServerName));
		Row->Setup(this, Index);
		Index++;

		ServerList->AddChild(Row);
	}

}

void UMainMenu::SelectServerIndex(uint32 ServerIndex)
{
	SelectedServerIndex = ServerIndex;

	for (int32 i = 0; i < ServerList->GetChildrenCount(); i++)
	{
		auto Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row != nullptr)
		{
			Row->isSelected = (ServerIndex == i);
		}
	}
	ConnectButton->SetIsEnabled(true);
}

bool UMainMenu::Initialize()
{
	bool SuperSucceeded = Super::Initialize();
	if (!SuperSucceeded) return false;

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);
	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	if (!ensure(OptionsButton != nullptr)) return false;
	OptionsButton->OnClicked.AddDynamic(this, &UMainMenu::OpenOptionsMenu);
	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitFromGame);
	if (!ensure(JoinBackButton != nullptr)) return false;
	JoinBackButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	if (!ensure(RefreshButton != nullptr)) return false;
	RefreshButton->OnClicked.AddDynamic(this, &UMainMenu::RefreshList);
	if (!ensure(ConnectButton != nullptr)) return false;
	ConnectButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	if (!ensure(HostConfirmButton != nullptr)) return false;
	HostConfirmButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	if (!ensure(HostBackButton != nullptr)) return false;
	HostBackButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	if (!ensure(OptionsBackButton != nullptr)) return false;
	OptionsBackButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	return true;
}

void UMainMenu::HostServer()
{
	if (!ensure(LobbyType != nullptr)) return;
	if (MenuInterface != nullptr)
	{
		MenuInterface->Host(LobbyType->GetSelectedOption());
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(JoinMenu);
	RefreshList();
}

void UMainMenu::OpenHostMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(HostMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::OpenOptionsMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(OptionsMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(OptionsMenu);
}

void UMainMenu::JoinServer()
{
	if (SelectedServerIndex.IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected server %d"), SelectedServerIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Selected Server..."));
	}
	if (MenuInterface != nullptr)
	{
		//if (!ensure(AddressField != nullptr)) return;
		//const FString &Address = AddressField->GetText().ToString();
		MenuInterface->Join(SelectedServerIndex.GetValue());

	}
}

void UMainMenu::RefreshList()
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	if (MenuInterface != nullptr)
	{
		MenuInterface->RefreshServerList();
		ConnectButton->SetIsEnabled(false);
		SelectedServerIndex.Reset();
		ServerList->ClearChildren();

		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->ServerName->SetText(FText::FromString("Searching for servers..."));
		Row->Setup(this, -1);

		ServerList->AddChild(Row);
	}
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitFromGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* LocalPlayerController = World->GetFirstPlayerController();
	if (!ensure(LocalPlayerController != nullptr)) return;

	LocalPlayerController->ConsoleCommand("quit");
}


