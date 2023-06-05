// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectBoatGameModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "ProjectBoatPlayerController.h"
#include "ProjectBoatPlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "CustomPlayerStart.h"
#include "Boat.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "EngineUtils.h"

template<typename T>
void FindAllActors(UWorld* World, TArray<T*>& Out)
{
	for (TActorIterator<T> It(World); It; ++It)
	{
		Out.Add(*It);
	}
}

AProjectBoatGameModeBase::AProjectBoatGameModeBase()
{
	ConstructorHelpers::FClassFinder<APawn> BoatBPClass(TEXT("/Game/ProjectBoat/Characters/BP_Player_Boat"));
	if (!ensure(BoatBPClass.Class != nullptr)) return;
	TSubclassOf<class APawn> BoatClass = BoatBPClass.Class;
	DefaultPawnClass = BoatClass;

	PlayerControllerClass = AProjectBoatPlayerController::StaticClass();

	PlayerStateClass = AProjectBoatPlayerState::StaticClass();
}

AActor* AProjectBoatGameModeBase::ChoosePlayerStart_Implementation(AController* Player) 
{
	TArray<ACustomPlayerStart*> FoundStarts;
	FindAllActors(GetWorld(), FoundStarts);

	for (int32 i = FoundStarts.Num() - 1; i > 0; i--) {
		int32 j = FMath::Floor(FMath::Rand() * (i + 1)) % FoundStarts.Num();
		ACustomPlayerStart* temp = FoundStarts[i];
		FoundStarts[i] = FoundStarts[j];
		FoundStarts[j] = temp;
	}

	TArray<ABoat*> FoundBoats;
	FindAllActors(GetWorld(), FoundBoats);

	ACustomPlayerStart* PlayerStart = nullptr;
	for (ACustomPlayerStart* CurrentStart : FoundStarts)
	{
		CurrentStart->CalculateSpawnScore(FoundBoats);

		if (PlayerStart == nullptr || PlayerStart->SpawnScore < CurrentStart->SpawnScore)
		{
			PlayerStart = CurrentStart;
		}
	}
	PlayerStart->ReserveSpawnPoint();
	return PlayerStart;
}

AActor* AProjectBoatGameModeBase::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	return ChoosePlayerStart(Player);
}

void AProjectBoatGameModeBase::HandleSeamlessTravelPlayer(AController*& C)
{
	Super::HandleSeamlessTravelPlayer(C);
	
	//UWidgetLayoutLibrary::RemoveAllWidgets(C);
	//APlayerController *PlayerController = Cast<APlayerController>(C);
	//if (PlayerController != nullptr)
	//{
	//	FInputModeGameOnly InputModeData;
	//	PlayerController->SetInputMode(InputModeData);
	//	PlayerController->bShowMouseCursor = false;
	//}

	LoadedPlayers++;
}

void AProjectBoatGameModeBase::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);

	LoadingPlayers++;
}

void AProjectBoatGameModeBase::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();

	//TArray<AProjectBoatPlayerController*> FoundPlayers;
	//FindAllActors(GetWorld(), FoundPlayers);
	//for (AProjectBoatPlayerController* PlayerController : FoundPlayers)
	//{
	//	UWidgetLayoutLibrary::RemoveAllWidgets(PlayerController);
	//
	//	FInputModeGameOnly InputModeData;
	//	PlayerController->SetInputMode(InputModeData);
	//	PlayerController->bShowMouseCursor = false;
	//}
}


