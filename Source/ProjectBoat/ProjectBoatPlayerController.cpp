// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBoatPlayerController.h"
#include "Boat.h"

AProjectBoatPlayerController::AProjectBoatPlayerController(const FObjectInitializer& ObjectIn) :
    APlayerController()
{
    bReplicates = true;
}

void AProjectBoatPlayerController::SetReady_Implementation()
{
    IsReady = !IsReady;
}

bool AProjectBoatPlayerController::GetReady()
{
    return IsReady;
}

void AProjectBoatPlayerController::SetLoadingComplete_Implementation()
{
    LoadingComplete = true;
}

bool AProjectBoatPlayerController::GetLoadingComplete()
{
    return LoadingComplete;
}