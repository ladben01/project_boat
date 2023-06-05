// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBoatPlayerState.h"

void AProjectBoatPlayerState::ToggleDebugMode()
{
    DebugMode = !DebugMode;
}

bool AProjectBoatPlayerState::GetDebugMode()
{
    return DebugMode;
}