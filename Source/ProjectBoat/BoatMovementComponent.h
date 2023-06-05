// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HUDWidget.h"
#include "BoatMovementComponent.generated.h"

USTRUCT()
struct FBoatMove
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY() float Throttle;
	UPROPERTY() float SteeringThrow;
	UPROPERTY() bool IsBoosting;
	UPROPERTY() float DeltaTime;
	UPROPERTY() float Time;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTBOAT_API UBoatMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBoatMovementComponent();

	void SetupMovement(FString ShipType);
	void SetHUD(UHUDWidget *_CurrentHUD) { CurrentHUD = _CurrentHUD; };
	FVector GetVelocity();
	void SetVelocity(FVector _Velocity);

	float GetThrottle() { return Throttle; };
	void SetThrottle(float _Throttle) { Throttle = _Throttle; };

	float GetSteeringThrow() { return SteeringThrow; };
	void SetSteeringThrow(float _SteeringThrow) { SteeringThrow = _SteeringThrow; };
	
	UFUNCTION(BlueprintCallable) bool GetIsBoosting() { return IsBoosting; };
	void SetIsBoosting(bool _IsBoosting) { IsBoosting = _IsBoosting; };

	UFUNCTION(BlueprintCallable) float GetCurrentBoost() { return CurrentBoost; };

	void Jump();
	void Dash();

	void SimulateMove(FBoatMove& Move);

	FBoatMove GetLastMove() { return LastMove; };

	UPROPERTY(BlueprintReadWrite) bool DebugMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float WaterLevel = 0; //Where the water is on the map //TODO make blueprint editable and set it in the level bp
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	//Mass in kg
	UPROPERTY(EditAnywhere, Category = "Movement Settings|Cargo Ship", DisplayName = "Mass")	float CSMass = 3000;
	UPROPERTY(EditAnywhere, Category = "Movement Settings|War Ship", DisplayName = "Mass")	float WSMass = 3000;
	float Mass = 3000;
	//Max force in N
	UPROPERTY(EditAnywhere, Category = "Movement Settings|Cargo Ship", DisplayName = "Max Force")	float CSMaxForce = 2000;
	UPROPERTY(EditAnywhere, Category = "Movement Settings|War Ship", DisplayName = "Max Force")	float WSMaxForce = 2000;
	float MaxForce = 2000;
	//Max rotation per second in degrees
	UPROPERTY(EditAnywhere, Category = "Movement Settings|Cargo Ship", DisplayName = "Max Rotation")	float CSMaxRotation = 40;
	UPROPERTY(EditAnywhere, Category = "Movement Settings|War Ship", DisplayName = "Max Rotation")	float WSMaxRotation = 40;
	float MaxRotation = 40;
	//Amount of drag that effects the boat
	UPROPERTY(EditAnywhere, Category = "Movement Settings|Cargo Ship", DisplayName = "Drag")	float CSDrag = 150;
	UPROPERTY(EditAnywhere, Category = "Movement Settings|War Ship", DisplayName = "Drag")	float WSDrag = 150;
	float Drag = 150;
	//How much the water effects the boat
	UPROPERTY(EditAnywhere, Category = "Movement Settings|Cargo Ship", DisplayName = "Water Coefficient")	float CSWaterCoefficient = 100;
	UPROPERTY(EditAnywhere, Category = "Movement Settings|War Ship", DisplayName = "Water Coefficient")	float WSWaterCoefficient = 100;
	float WaterCoefficient = 100;
	//How much the steering is delayed
	UPROPERTY(EditAnywhere, Category = "Movement Settings|Cargo Ship", DisplayName = "Rotation Accelaration") float CSRotationAccelaration = 1.2;
	UPROPERTY(EditAnywhere, Category = "Movement Settings|War Ship", DisplayName = "Rotation Accelaration") float WSRotationAccelaration = 1.2;
	float RotationAccelaration = 1.2;
	//How much the ship is steering back
	UPROPERTY(EditAnywhere, Category = "Movement Settings|Cargo Ship", DisplayName = "Rotation Decelaration") float CSRotationDecelaration = 1.1;
	UPROPERTY(EditAnywhere, Category = "Movement Settings|War Ship", DisplayName = "Rotation Decelaration") float WSRotationDecelaration = 1.1;
	float RotationDecelaration = 1.1;
	//How much force the ship gets when jumping
	UPROPERTY(EditAnywhere, Category = "Movement Settings") float JumpForce = 150000;

	float MaxVelocityRotationDelay = 10000;
	float VelocityRotationDelay = 10000;


	float Throttle = 0;
	float SteeringThrow = 0;
	bool IsBoosting = false;

	float MaxBoost = 100;
	float CurrentBoost = MaxBoost;
	// How much boost is used per sec
	float BoostStrength = 30;
	// How much boost is regenerated per sec
	float BoostRegen = 6;
	// How much time does it take to start regenerating boost (s)
	float MaxBoostRegenTimer = 2;
	float BoostRegenTimer = MaxBoostRegenTimer;

	float JumpCooldown = 1; //in s
	float CurrentJumpCooldown = 0;
	bool InAir = false;

	float Rotation = 0;
	float ForwardDir = 1;

	FVector Velocity;
	FVector Direction;

	FBoatMove LastMove;

	FBoatMove CreateMove(float DeltaTime);
	void ApplyRotation(FBoatMove Move);
	void UpdateLocationFromVelocity(FBoatMove Move);

	UStaticMeshComponent* BoatMesh;
	UHUDWidget* CurrentHUD = nullptr;
};
