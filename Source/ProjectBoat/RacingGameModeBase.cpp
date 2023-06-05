// Fill out your copyright notice in the Description page of Project Settings.


#include "RacingGameModeBase.h"
#include "RacePlayerStart.h"
#include "ProjectBoatPlayerController.h"
#include "ProjectBoatPlayerState.h"
#include "EngineUtils.h"

template<typename T>
void FindAllActors(UWorld* World, TArray<T*>& Out)
{
	for (TActorIterator<T> It(World); It; ++It)
	{
		Out.Add(*It);
	}
}

ARacingGameModeBase::ARacingGameModeBase()
{
	ConstructorHelpers::FClassFinder<APawn> BoatBPClass(TEXT("/Game/ProjectBoat/Characters/BP_Player_Boat"));
	if (!ensure(BoatBPClass.Class != nullptr)) return;
	TSubclassOf<class APawn> BoatClass = BoatBPClass.Class;
	DefaultPawnClass = BoatClass;

	PlayerControllerClass = AProjectBoatPlayerController::StaticClass();

	PlayerStateClass = AProjectBoatPlayerState::StaticClass();
}

AActor* ARacingGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<ARacePlayerStart*> FoundStarts;
	FindAllActors(GetWorld(), FoundStarts);

	for (int32 i = FoundStarts.Num() - 1; i > 0; i--) {
		int32 j = FMath::Floor(FMath::Rand() * (i + 1)) % FoundStarts.Num();
		ARacePlayerStart* temp = FoundStarts[i];
		FoundStarts[i] = FoundStarts[j];
		FoundStarts[j] = temp;
	}

	ARacePlayerStart* PlayerStart = nullptr;
	for (ARacePlayerStart* CurrentStart : FoundStarts)
	{

		if (CurrentStart->IsStartingPoint && CurrentStart->IsSpawnPointAvailable())
		{
			PlayerStart = CurrentStart;
			PlayerStart->ReserveSpawnPoint();
			return PlayerStart;
		}
	}
	return nullptr;
	
}

AActor* ARacingGameModeBase::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	return ChoosePlayerStart(Player);
}

void ARacingGameModeBase::HandleSeamlessTravelPlayer(AController*& C)
{
	Super::HandleSeamlessTravelPlayer(C);

	LoadedPlayers++;
}

void ARacingGameModeBase::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);

	LoadingPlayers++;
}

void ARacingGameModeBase::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();

}