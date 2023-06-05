// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WarningWidget.generated.h"

/**
 *
 */
UCLASS()
class PROJECTBOAT_API UWarningWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup(const FString& _Message, const FString& _ButtonText);
	void Teardown();
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld);
protected:
	virtual bool Initialize();
private:
	UPROPERTY(meta = (BindWidget)) class UTextBlock* WarningMessage;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ButtonText;
	UPROPERTY(meta = (BindWidget)) class UButton* WarningButton;

	UFUNCTION() void WarningButtonClick();

};
