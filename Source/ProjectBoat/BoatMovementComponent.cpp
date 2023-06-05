// Fill out your copyright notice in the Description page of Project Settings.


#include "BoatMovementComponent.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "Math/Vector.h"
#include "Math/UnrealMathUtility.h"

#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UBoatMovementComponent::UBoatMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UBoatMovementComponent::SetupMovement(FString ShipType)
{
	if (ShipType == "Cargoship")
	{
		Mass = CSMass;
		MaxForce = CSMaxForce;
		MaxRotation = CSMaxRotation;
		Drag = CSDrag;
		WaterCoefficient = CSWaterCoefficient;
		RotationAccelaration = CSRotationAccelaration;
		RotationDecelaration = CSRotationDecelaration;
	}
	else if (ShipType == "Warship")
	{
		Mass = WSMass;
		MaxForce = WSMaxForce;
		MaxRotation = WSMaxRotation;
		Drag = WSDrag;
		WaterCoefficient = WSWaterCoefficient;
		RotationAccelaration = WSRotationAccelaration;
		RotationDecelaration = WSRotationDecelaration;
	}
}

// Called when the game starts
void UBoatMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	BoatMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	WaterLevel += 100; //adjust for ship center
}


// Called every frame
void UBoatMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	auto MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn == nullptr) return;

	if (MyPawn->IsLocallyControlled())
	{
		LastMove = CreateMove(DeltaTime);
		SimulateMove(LastMove);
		
		if (CurrentJumpCooldown != 0)
		{
			if (CurrentJumpCooldown > DeltaTime)
			{
				CurrentJumpCooldown -= DeltaTime;
			}
			else
			{
				CurrentJumpCooldown = 0;
			}
		}

		if (InAir && GetOwner()->GetActorLocation().Z <= WaterLevel && CurrentJumpCooldown == 0)
		{
			InAir = false;
		}

		if(CurrentHUD != nullptr && (CurrentBoost != 0 || IsBoosting))
			CurrentHUD->SetBoost(MaxBoost,CurrentBoost);
	}

	//UEngine* Engine = GetOwner()->GetGameInstance()->GetEngine();
	//if (!ensure(Engine != nullptr)) return;
	//if(DebugMode) Engine->AddOnScreenDebugMessage(0, DeltaTime, FColor::Green, FString::Printf(TEXT("Ship Bottom Z Location: %f"), GetOwner()->GetActorLocation().Z));
	//Engine->AddOnScreenDebugMessage(0, DeltaTime, FColor::Green, FString::Printf(TEXT("InAir: %s"), InAir ? TEXT("True") : TEXT("False")));
	//Engine->AddOnScreenDebugMessage(0, DeltaTime, FColor::Green, FString::Printf(TEXT("JumpCooldown: %f"), CurrentJumpCooldown));
}

//Creates a move to simulate
FBoatMove UBoatMovementComponent::CreateMove(float DeltaTime)
{
	FBoatMove Move;
	Move.DeltaTime = DeltaTime;
	Move.SteeringThrow = SteeringThrow;
	Move.Throttle = Throttle;
	Move.IsBoosting = IsBoosting;
	Move.Time = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

	return Move;
}

//Calculates player movement
void UBoatMovementComponent::SimulateMove(FBoatMove& Move)
{
	float RotDir = 0;
	if (Rotation > 0)
		RotDir = 1;
	else if (Rotation < 0)
		RotDir = -1;
	
	/*if (Rotation) //+ VelocityRotationDelay != 0)
	{
		float RotStrength = abs(Rotation);
		if (abs(Rotation) > 1) RotStrength = 1;

		//VelocityRotationDelay--;

		Direction = GetOwner()->GetActorForwardVector();//(GetOwner()->GetActorRightVector() * RotStrength * RotDir + GetOwner()->GetActorForwardVector()).GetSafeNormal();
	}
	else */Direction = GetOwner()->GetActorForwardVector();
	Direction.Z = 0;


	if (DebugMode)
	{
		FVector DebugStart = GetOwner()->GetActorLocation();
		DebugStart.Z += 800;
		FVector DebugEnd = DebugStart + Direction * 10000;
		DrawDebugLine(GetWorld(), DebugStart, DebugEnd, FColor::Green, false, -1.0F, 0U, 100.0F);
		DebugEnd = DebugStart + BoatMesh->GetPhysicsLinearVelocity() * 10000;
		DebugEnd.Z = DebugStart.Z;
		DrawDebugLine(GetWorld(), DebugStart, DebugEnd, FColor::Red, false, -1.0F, 0U, 50.0F);
	}

	
	UpdateLocationFromVelocity(Move);
	ApplyRotation(Move);
}


void UBoatMovementComponent::ApplyRotation(FBoatMove Move)
{
	if (Move.Throttle != 0 || IsBoosting)
	{
		float Dir = FVector::DotProduct(GetOwner()->GetActorForwardVector(), BoatMesh->GetPhysicsLinearVelocity());
		if (Dir < -0.2)
			ForwardDir = -1;
		else if (Dir > 0.2)
			ForwardDir = 1;
	}

	if (Move.SteeringThrow != 0)
	{
		Rotation = Rotation + RotationAccelaration * Move.SteeringThrow * ForwardDir;
		if (abs(Rotation) > MaxRotation)
		{
			if (Rotation > 0)
				Rotation = MaxRotation;
			else Rotation = -MaxRotation;
		}
	}
	else if (Rotation > RotationDecelaration)
		Rotation = Rotation - RotationDecelaration;
	else if (Rotation < -RotationDecelaration)
		Rotation = Rotation + RotationDecelaration;
	else Rotation = 0;
	
	//Rotation = MaxRotation * Move.SteeringThrow * ForwardDir;
	//if(Rotation != 0) VelocityRotationDelay = MaxVelocityRotationDelay;

	//float RotationSpeed = BoatMesh->GetPhysicsLinearVelocity().Size() /27.777777777 / 55.0;
	float RotationAngle = Rotation * Move.DeltaTime * 1000;// *2;
	//FVector ShipRotator = FVector(0, 0, RotationAngle);
	FRotator ShipRotator = FRotator(0, Rotation, 0);
	FRotator TargetRotator = FMath::RInterpTo(FRotator(0,0,0), ShipRotator, Move.DeltaTime, .5f);
	GetOwner()->AddActorWorldRotation(TargetRotator, false, nullptr, ETeleportType::None);
	if (Move.Throttle >= 0 || IsBoosting || Move.SteeringThrow == 0) BoatMesh->SetPhysicsLinearVelocity(TargetRotator.RotateVector(BoatMesh->GetPhysicsLinearVelocity()));
	//FRotator TargetRotator = FMath::RInterpConstantTo(GetOwner()->GetActorRotation(), GetOwner()->GetActorRotation() + ShipRotator, Move.DeltaTime, 5.0f);
	
	//GetOwner()->SetActorRotation(TargetRotator, ETeleportType::None);
	//BoatMesh->AddTorqueInRadians(ShipRotator,NAME_None, true);
}

void UBoatMovementComponent::UpdateLocationFromVelocity(FBoatMove Move)
{
	float NormalForce = -GetWorld()->GetGravityZ() / 100 / Mass;
	FVector NormalVelocity = BoatMesh->GetPhysicsLinearVelocity().GetSafeNormal() / 100;
	NormalVelocity.Z = 0;

	FVector AirResistance = -NormalVelocity * (BoatMesh->GetPhysicsLinearVelocity() / 100).SizeSquared() * Drag;
	FVector WaterResistance = -NormalVelocity * NormalForce * WaterCoefficient;

	if (Move.SteeringThrow != 0)
		WaterResistance = WaterResistance * abs(SteeringThrow) * 100;

	FVector Accelaration = (Direction * MaxForce * Move.Throttle + AirResistance + WaterResistance) / Mass;

	float Dir = FVector::DotProduct(GetOwner()->GetActorForwardVector(), BoatMesh->GetPhysicsLinearVelocity());
	if (Move.Throttle < 0 && Dir < 0)
		Accelaration = (Direction * MaxForce * Move.Throttle / 3 + AirResistance + WaterResistance) / Mass;

	Velocity = Accelaration;
	FVector Translation = Accelaration * 100 * 1000;

	if (Move.IsBoosting)
	{
		if (CurrentBoost > BoostStrength * Move.DeltaTime)
		{
			Translation += Direction * MaxForce / Mass * 100 * 5500;
			CurrentBoost -= BoostStrength * Move.DeltaTime;
			BoostRegenTimer = MaxBoostRegenTimer;
		}
		else
		{
			CurrentBoost = 0;
			IsBoosting = false;
		}
		
	}
	else if (BoostRegenTimer > 0)
	{
		BoostRegenTimer -= Move.DeltaTime;
	}
	else if (CurrentBoost < MaxBoost)
	{
		CurrentBoost += BoostRegen * Move.DeltaTime;
		if (CurrentBoost > MaxBoost) CurrentBoost = MaxBoost;
	}
		

	//BoatMesh->AddForce(Translation);
	BoatMesh->AddForceAtLocation(Translation,BoatMesh->GetCenterOfMass());
}

void UBoatMovementComponent::Jump()
{
	auto MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn == nullptr) return;

	if (MyPawn->IsLocallyControlled() && CurrentJumpCooldown == 0 && !InAir)
	{
		CurrentJumpCooldown = JumpCooldown;
		InAir = true;

		BoatMesh->AddImpulseAtLocation(FVector(0, 0, JumpForce), BoatMesh->GetCenterOfMass());
	}
}

void UBoatMovementComponent::Dash()
{
	auto MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn == nullptr) return;

	if (MyPawn->IsLocallyControlled())
	{
		FVector DashForce = GetOwner()->GetActorForwardVector();
		DashForce.Z = 0;
		DashForce = DashForce * 150000;
		BoatMesh->AddImpulseAtLocation(DashForce, BoatMesh->GetCenterOfMass());
	}
}

FVector UBoatMovementComponent::GetVelocity()
{
	return BoatMesh->GetPhysicsLinearVelocity();
}

void UBoatMovementComponent::SetVelocity(FVector _Velocity)
{
	BoatMesh->SetPhysicsLinearVelocity(_Velocity);
}