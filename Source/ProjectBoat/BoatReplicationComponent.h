// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BoatMovementComponent.h"
#include "ProjectBoatGameInstance.h"
#include "BoatReplicationComponent.generated.h"

USTRUCT()
struct FBoatState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY() FVector Velocity;
	UPROPERTY() FTransform Transform;
	UPROPERTY() FBoatMove LastMove;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTBOAT_API UBoatReplicationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBoatReplicationComponent();
	void OnScoreChanged(int32 _Score);
	void OnShipTypeChanged(TEnumAsByte<ShipTypes> _ShipType);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TArray<FBoatMove> UnAcknowledgedMoves;

	float ClientTimeSinceLastUpdate = 0;
	float ClientTimeBetweenLastUpdates = 0;
	FTransform ClientStartTransform;
	FVector ClientStartVelocity;

	void ClearAcknowledgedMoves(FBoatMove LastMove);

	UPROPERTY(ReplicatedUsing = OnRepScore) int32 Score;
	UFUNCTION() void OnRepScore();
	void UpdateScore(int32 _Score);
	UFUNCTION(Server, Reliable) void Server_SendScore(int32 _Score);

	UPROPERTY(ReplicatedUsing = OnRepServerState) FBoatState ServerState;
	UFUNCTION() void OnRepServerState();
	//void AutonomousProxyOnRepServerState();
	void SimulatedProxyOnRepServerState();

	UPROPERTY(ReplicatedUsing = OnRepShipType) TEnumAsByte<ShipTypes> ShipType = WarShip;
	UFUNCTION() void OnRepShipType();
	void UpdateShipType(const TEnumAsByte<ShipTypes> _ShipType);
	UFUNCTION(Server, Reliable) void Server_ChangeShipType(ShipTypes _ShipType);

	//void AutonomousClientTick(float DeltaTime);
	void SimulatedClientTick(float DeltaTime);

	void UpdateServerState(FBoatMove Move, FVector _Velocity = FVector::ZeroVector, FTransform _Transform = FTransform());
	UFUNCTION(Server, Reliable, WithValidation) void Server_SendMove(FVector _Velocity, FTransform _Transform, FBoatMove _Move);

	UPROPERTY(EditAnywhere) UBoatMovementComponent* MovementComponent;
};
