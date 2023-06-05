// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.h"
#include "ServerRow.generated.h"

/**
 *
 */
UCLASS()
class PROJECTBOAT_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ServerName;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ServerPopulation;

	UPROPERTY(BlueprintReadOnly) bool isSelected = false;

	void Setup(UMainMenu* _Parent, uint32 _Index);

private:
	UMainMenu* Parent;
	uint32 Index;

	UPROPERTY(meta = (BindWidget)) class UButton* ServerRowButton;

	UFUNCTION() void OnClicked();
};
