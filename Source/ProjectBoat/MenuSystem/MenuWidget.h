// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuWidget.generated.h"

/**
 *
 */
UCLASS()
class PROJECTBOAT_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuInterface(IMenuInterface* _MenuInterface);
	void Setup();
	void Teardown();
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld);
protected:
	IMenuInterface* MenuInterface;
};
