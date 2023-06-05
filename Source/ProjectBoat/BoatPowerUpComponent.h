// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BoatType.h"
#include "BoatMovementComponent.h"
#include "PowerUps.h"
#include "BoatPowerUpComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTBOAT_API UBoatPowerUpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBoatPowerUpComponent();
	void SetupComponent(ShipTypes _ShipType, TArray<FVector> _MissilePoints, TArray<FVector> _TorpedoPoints);
	void SetHUD(UHUDWidget* _CurrentHUD) { CurrentHUD = _CurrentHUD; if (CurrentHUD != nullptr) CurrentHUD->SetPowerUps(PowerUps); };

	void PickupRandom();
	void AddPowerUp(PowerUpTypes Type, int Count);
	void UsePowerUp(int Slot, FTransform CameraTransform);

	AActor* SpawnPowerUpActor(FPowerUps PowerUp, FVector Location, FRotator Rotation);
	UFUNCTION(Server, Reliable) void Server_SpawnPowerUpActor(FPowerUps PowerUp, FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintCallable) void OnPowerUpActorDestroyed(AActor* Actor);

	void RemovePowerUp(FPowerUps PowerUp);
	UFUNCTION(Client, Reliable) void Client_RemovePowerUp(FPowerUps PowerUp);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TEnumAsByte<ShipTypes> ShipType = WarShip;

	TArray<FPowerUps> PowerUps;
	int FixPowerUps = 0;
	int MaxPowerUps = 2;

	TArray<FVector> MissilePoints;
	TArray<FVector> TorpedoPoints;
	UPROPERTY(EditAnywhere) UBoatMovementComponent* MovementComponent;
	UHUDWidget* CurrentHUD = nullptr;
};
