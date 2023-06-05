// Fill out your copyright notice in the Description page of Project Settings.


#include "BoatReplicationComponent.h"
#include "Boat.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UBoatReplicationComponent::UBoatReplicationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UBoatReplicationComponent::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent = GetOwner()->FindComponentByClass<UBoatMovementComponent>();
	
}


// Called every frame
void UBoatReplicationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MovementComponent == nullptr) return;

	ABoat* MyPawn = Cast<ABoat>(GetOwner());
	if (MyPawn == nullptr) return;

	FBoatMove Move = MovementComponent->GetLastMove();

	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		
		//UnAcknowledgedMoves.Add(Move);
		Server_SendMove(MovementComponent->GetVelocity(), GetOwner()->GetTransform(), Move);
	}
	if (GetOwnerRole() == ROLE_Authority)
	{
		if (MyPawn->IsLocallyControlled())
		{
			UpdateServerState(Move, MovementComponent->GetVelocity(), GetOwner()->GetActorTransform());
		}
		else
		{
			//SimulatedClientTick(DeltaTime);
			//FQuat TargetRotation = ServerState.Transform.GetRotation();
			if (ServerState.Transform.IsValid())
			{
				//	GetOwner()->SetActorTransform(ServerState.Transform);
				FVector TargetLocation = ServerState.Transform.GetLocation();
				if (!TargetLocation.IsZero())
				{
					FVector StartLocation = ClientStartTransform.GetLocation();

					FVector NewLocation = FMath::LerpStable(StartLocation, TargetLocation, 1.0);

					GetOwner()->SetActorLocation(NewLocation);

					FQuat StartRotation = ClientStartTransform.GetRotation();
					FQuat TargetRotation = ServerState.Transform.GetRotation();

					FQuat NewRotation = FQuat::Slerp(StartRotation, TargetRotation, 1.0);

					GetOwner()->SetActorRotation(NewRotation);
				}
			}
		}
	}
	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		SimulatedClientTick(DeltaTime);
	}
}

void UBoatReplicationComponent::SimulatedClientTick(float DeltaTime)
{
	ClientTimeSinceLastUpdate += DeltaTime;

	if (ClientTimeBetweenLastUpdates < KINDA_SMALL_NUMBER) return;
	if (MovementComponent == nullptr) return;

	float Ratio = ClientTimeSinceLastUpdate / ClientTimeBetweenLastUpdates;

	FVector StartLocation = ClientStartTransform.GetLocation();
	FVector StartVelocity = ClientStartVelocity;
	FVector TargetLocation = ServerState.Transform.GetLocation();
	FVector TargetVelocity = ServerState.Velocity * ClientTimeBetweenLastUpdates * 100; // * 100 mert konvertálni kell a velocityt m/s^2-bõl cm/s^2
	
	FVector NewLocation = FMath::LerpStable(StartLocation, TargetLocation, Ratio);

	GetOwner()->SetActorLocation(NewLocation);

	FQuat StartRotation = ClientStartTransform.GetRotation();
	FQuat TargetRotation = ServerState.Transform.GetRotation();
	
	FQuat NewRotation = FQuat::Slerp(StartRotation, TargetRotation, Ratio);
	
	GetOwner()->SetActorRotation(NewRotation);
}

void UBoatReplicationComponent::UpdateServerState(FBoatMove Move, FVector _Velocity, FTransform _Transform)
{
	ServerState.Transform = _Transform;
	ServerState.Velocity = _Velocity;
	ServerState.LastMove = Move;
}

void UBoatReplicationComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UBoatReplicationComponent, ServerState);
	DOREPLIFETIME(UBoatReplicationComponent, Score);
	DOREPLIFETIME(UBoatReplicationComponent, ShipType);
}

void UBoatReplicationComponent::ClearAcknowledgedMoves(FBoatMove LastMove)
{
	TArray<FBoatMove> NewMoves;

	for (FBoatMove& Move : UnAcknowledgedMoves)
	{
		if (Move.Time > LastMove.Time)
			NewMoves.Add(Move);
	}

	UnAcknowledgedMoves = NewMoves;
}

void UBoatReplicationComponent::OnRepServerState()
{
	if(GetOwnerRole() == ROLE_SimulatedProxy)
		SimulatedProxyOnRepServerState();
}

void UBoatReplicationComponent::SimulatedProxyOnRepServerState()
{
	if (MovementComponent == nullptr) return;

	if (ClientTimeSinceLastUpdate > KINDA_SMALL_NUMBER)
	{
		ClientTimeBetweenLastUpdates = ClientTimeSinceLastUpdate;
		ClientTimeSinceLastUpdate = 0;
	}

	ClientStartTransform = GetOwner()->GetActorTransform();
	ClientStartVelocity = MovementComponent->GetVelocity();
}

void UBoatReplicationComponent::Server_SendMove_Implementation(FVector _Velocity, FTransform _Transform, FBoatMove _Move)
{
	if (MovementComponent == nullptr) return;

	ClientStartTransform = GetOwner()->GetActorTransform();
	ClientStartVelocity = MovementComponent->GetVelocity();
	MovementComponent->SetVelocity(_Velocity);

	UpdateServerState(_Move, _Velocity, _Transform);
}

bool UBoatReplicationComponent::Server_SendMove_Validate(FVector _Velocity, FTransform _Transform, FBoatMove Move)
{
	return FMath::Abs(MovementComponent->GetThrottle()) <= 1;
	return FMath::Abs(MovementComponent->GetSteeringThrow()) <= 1;
}

void UBoatReplicationComponent::OnScoreChanged(int32 _Score)
{
	auto MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn == nullptr) return;

	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		Server_SendScore(_Score);
	}
	if (GetOwnerRole() == ROLE_Authority)
	{
		UpdateScore(_Score);
	}
}

void UBoatReplicationComponent::UpdateScore(int32 _Score)
{
	ABoat *MyPawn = Cast<ABoat>(GetOwner());
	if (MyPawn == nullptr) return;

	Score = _Score;
	MyPawn->SetScore(_Score);
}

void UBoatReplicationComponent::OnRepScore()
{
	ABoat* MyPawn = Cast<ABoat>(GetOwner());
	if (MyPawn == nullptr) return;

	MyPawn->SetScore(Score);
}

void UBoatReplicationComponent::Server_SendScore_Implementation(int32 _Score)
{
	UpdateScore(_Score);
}

void UBoatReplicationComponent::OnShipTypeChanged(TEnumAsByte<ShipTypes> _ShipType)
{
	auto MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn == nullptr) return;

	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		Server_ChangeShipType(_ShipType);
	}
	if (GetOwnerRole() == ROLE_Authority)
	{
		UpdateShipType(_ShipType);
	}
}

void UBoatReplicationComponent::UpdateShipType(const TEnumAsByte<ShipTypes> _ShipType)
{
	ABoat* MyPawn = Cast<ABoat>(GetOwner());
	if (MyPawn == nullptr) return;

	ShipType = _ShipType;
	MyPawn->SetShipType(_ShipType);
}

void UBoatReplicationComponent::OnRepShipType()
{
	ABoat* MyPawn = Cast<ABoat>(GetOwner());
	if (MyPawn == nullptr) return;

	MyPawn->SetShipType(ShipType);
}

void UBoatReplicationComponent::Server_ChangeShipType_Implementation(ShipTypes _ShipType)
{
	UpdateShipType(_ShipType);
}