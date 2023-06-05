// Fill out your copyright notice in the Description page of Project Settings.


#include "Port.h"

// Sets default values
APort::APort()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(4000, 2000, 300));
	SetRootComponent(BoxCollision);

}

// Called when the game starts or when spawned
void APort::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APort::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

