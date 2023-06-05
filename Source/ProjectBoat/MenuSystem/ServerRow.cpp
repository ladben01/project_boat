// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UServerRow::Setup(UMainMenu* _Parent, uint32 _Index)
{
	Parent = _Parent;
	Index = _Index;

	if (!ensure(ServerRowButton != nullptr)) return;
	if (Index == -1)
	{
		ServerPopulation->SetText(FText::FromString(""));
	}
	else
	{
		ServerRowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
	}
}

void UServerRow::OnClicked()
{
	Parent->SelectServerIndex(Index);
}
