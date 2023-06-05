// Fill out your copyright notice in the Description page of Project Settings.


#include "Boat.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "ProjectBoatPlayerState.h"
#include "HUDWidget.h"
#include "ProjectileBase.h"
#include "PowerUpPickup.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABoat::ABoat()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(false);
	bAlwaysRelevant = true;

	BoatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoatMesh"));
	SetRootComponent(BoatMesh);

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(BoatMesh);
	CameraArm->SetRelativeRotation(FRotator(-20, 0, 0));
	CameraArm->bInheritPitch = false;
	CameraArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm,USpringArmComponent::SocketName);

	MovementComponent = CreateDefaultSubobject<UBoatMovementComponent>(TEXT("MovementComponent"));
	ReplicationComponent = CreateDefaultSubobject<UBoatReplicationComponent>(TEXT("ReplicationComponent"));
	BuoyancyComponent = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("BuoyancyComponent"));

	BuoyancyComponent->SetAutoActivate(true);
	BuoyancyComponent->BuoyancyData.BuoyancyDamp2 = 1.5;
	BuoyancyComponent->BuoyancyData.BuoyancyRampMinVelocity = 5;
	BuoyancyComponent->BuoyancyData.BuoyancyRampMaxVelocity = 20;
	BuoyancyComponent->BuoyancyData.BuoyancyRampMax = 2;
	BuoyancyComponent->BuoyancyData.bApplyDragForcesInWater = true;

	CargoshipBuoyancyPoints.Add(FVector(1010.0f, -290.0f, 165.0f));
	CargoshipBuoyancyPoints.Add(FVector(1010.0f, 290.0f, 165.0f));
	CargoshipBuoyancyPoints.Add(FVector(-1010.0f, -290.0f, 165.0f));
	CargoshipBuoyancyPoints.Add(FVector(-1010.0f, 290.0f, 165.0f));

	WarshipBuoyancyPoints.Add(FVector(970.0f, -140.0f, 0.0f));
	WarshipBuoyancyPoints.Add(FVector(970.0f, 140.0f, 0.0f));
	WarshipBuoyancyPoints.Add(FVector(-970.0f, -140.0f, 0.0f));
	WarshipBuoyancyPoints.Add(FVector(-970.0f, 140.0f, 0.0f));

	Package = CreateDefaultSubobject<UBoatPackageComponent>(TEXT("Package"));
	
	CargoshipPackages.Add(FVector(0, 0, 1000));
	CargoshipPackages.Add(FVector(100, 0, 1000));

	WarshipPackages.Add(FVector(-160, 40, 253));

	PowerUpComponent = CreateDefaultSubobject<UBoatPowerUpComponent>(TEXT("PowerUpComponent"));

	CargoshipMissilePoints.Add(FVector(1650, 0, 1000));
	CargoshipTorpedoPoints.Add(FVector(0, 400, 165));
	CargoshipTorpedoPoints.Add(FVector(0, -400, 165));

	WarshipMissilePoints.Add(FVector(0, 0, 1000));
	WarshipTorpedoPoints.Add(FVector(0, 150, 165));
	WarshipTorpedoPoints.Add(FVector(0, -150, 165));

	SetupShip(ShipType);
	MovementComponent->SetupMovement(GetShipTypeAsString(ShipType));

	BoatMesh->SetSimulatePhysics(true);
	BoatMesh->BodyInstance.bOverrideMass = true;
	BoatMesh->BodyInstance.SetMassOverride(100.0F);
	BoatMesh->SetAngularDamping(1.0F);
	BoatMesh->SetNotifyRigidBodyCollision(true);

	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));

	static ConstructorHelpers::FObjectFinder<UMaterial>OFBoostPostProcessAsset(TEXT("Material'/Game/ProjectBoat/Assets/Mats/Mat_BoostScreenEffect.Mat_BoostScreenEffect'"));
	BoostPostProcessMaterial = OFBoostPostProcessAsset.Object;

	BoatMesh->OnComponentBeginOverlap.AddDynamic(this, &ABoat::OnOverlapBegin);
	BoatMesh->OnComponentEndOverlap.AddDynamic(this, &ABoat::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ABoat::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocallyControlled())
	{
		CurrentHUD = GetGameInstance<UProjectBoatGameInstance>()->GetHUDWidget();
		MovementComponent->SetHUD(CurrentHUD);
		Package->SetHUD(CurrentHUD);
		PowerUpComponent->SetHUD(CurrentHUD);
	}

	BoostPostProcess = BoostPostProcess->Create(BoostPostProcessMaterial, this);

	if (HasAuthority())
	{
		NetUpdateFrequency = 30;
		if (!IsLocallyControlled())
		{
			BoatMesh->SetSimulatePhysics(false);
		}
	}

	if (GetLocalRole() == ROLE_SimulatedProxy)// && GetRemoteRole() == ROLE_SimulatedProxy)
	{
		BoatMesh->SetSimulatePhysics(false);
	}
	
	if (IsLocallyControlled())
	{
		ChangeShipType(GetGameInstance<UProjectBoatGameInstance>()->GetShipType());
		
		if (BoostPostProcess != nullptr) PostProcessComponent->AddOrUpdateBlendable(BoostPostProcess);
	}
	

	SetupShip(ShipType);
	MovementComponent->SetupMovement(GetShipTypeAsString(ShipType));
}

void ABoat::Respawn(FVector RespawnLocation, FRotator RespawnRotation)
{
	BoatMesh->SetSimulatePhysics(false);
	RemovePackage();
	AddScore(-100);
	SetHealth(10);
	SetActorLocationAndRotation(RespawnLocation,RespawnRotation,false,nullptr,ETeleportType::TeleportPhysics);
	BoatMesh->SetSimulatePhysics(true);
	BuoyancyComponent->Activate();
	MovementComponent->Activate();
	IsRespawning = false;
}

void ABoat::Die(FVector RespawnLocation, FRotator RespawnRotation)
{
	if (!IsRespawning)
	{
		IsRespawning = true;
		BuoyancyComponent->Deactivate();
		MovementComponent->Deactivate();
		FTimerHandle UnusedHandle;
		FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(this, &ABoat::Respawn, RespawnLocation, RespawnRotation);
		GetWorldTimerManager().SetTimer(UnusedHandle, RespawnDelegate, 5.0f, false);
	}
}

// Called every frame
void ABoat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementComponent != nullptr)
	{
		if (MovementComponent->DebugMode)
		{
			DrawDebugString(GetWorld(), FVector(0, 0, 100), UEnum::GetValueAsString(GetLocalRole()), this, FColor::White, 0);

			FString PlayerName = GetPlayerState<AProjectBoatPlayerState>()->GetPlayerName();
			DrawDebugString(GetWorld(), FVector(0, 0, 3000), PlayerName, this, FColor::White, 0);
		}

		if (IsLocallyControlled() && BoostPostProcess != nullptr && MovementComponent->GetIsBoosting())
		{
			BoostPostProcess->SetScalarParameterValue(FName("Weight"), 1.0f);
		}
		else
		{
			BoostPostProcess->SetScalarParameterValue(FName("Weight"), 0.0f);
		}
	}
	if (HitTimer > 0) HitTimer -= DeltaTime;
		
}

// Called to bind functionality to input
void ABoat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABoat::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABoat::MoveRight);
	PlayerInputComponent->BindAction("PrimaryPowerUp", EInputEvent::IE_Pressed, this, &ABoat::UsePrimaryPowerUp);
	PlayerInputComponent->BindAction("SecondaryPowerUp", EInputEvent::IE_Pressed, this, &ABoat::UseSecondaryPowerUp);
	PlayerInputComponent->BindAction("UseBoost", EInputEvent::IE_Pressed, this, &ABoat::UseBoost);
	PlayerInputComponent->BindAction("UseBoost", EInputEvent::IE_Released, this, &ABoat::StopBoost);
	PlayerInputComponent->BindAction("PackagePickup", EInputEvent::IE_Pressed, this, &ABoat::PackagePickup);
	PlayerInputComponent->BindAction("PackageDropoff", EInputEvent::IE_Pressed, this, &ABoat::PackageDropoff);
}

void ABoat::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<APort>())
	{
		CurrentPort = Cast<APort,AActor>(OtherActor);
		if (CurrentHUD != nullptr)
		{
			CurrentHUD->SetHintText("Press 'E' to pick up or 'F' to drop off package");
		}
		//DrawDebugString(GetWorld(), FVector(0, 0, 300), CurrentPort->GetPortName(), this, FColor::White, 1);
	}
	else if (OtherActor->IsA<APowerUpPickup>())
	{
		if (Cast<APowerUpPickup, AActor>(OtherActor)->CanPickUp)
		{
			if (PowerUpComponent != nullptr)
			{
				PowerUpComponent->PickupRandom();
			}
		}
	}
}

void ABoat::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<APort>())
	{
		CurrentPort = nullptr;
		if (CurrentHUD != nullptr)
		{
			CurrentHUD->SetHintText("");
		}
	}
}

void ABoat::MoveForward(float Value)
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SetThrottle(Value);
}

void ABoat::MoveRight(float Value)
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SetSteeringThrow(Value);
}

void ABoat::Jump()
{
	if (MovementComponent == nullptr) return;

	MovementComponent->Jump();
}

void ABoat::UseBoost()
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SetIsBoosting(true);
}

void ABoat::StopBoost()
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SetIsBoosting(false);
}

void ABoat::UsePrimaryPowerUp()
{
	if (PowerUpComponent == nullptr) return;

	PowerUpComponent->UsePowerUp(0, Camera->GetComponentTransform());
}

void ABoat::UseSecondaryPowerUp()
{
	if (PowerUpComponent == nullptr) return;

	PowerUpComponent->UsePowerUp(1, Camera->GetComponentTransform());
}

void ABoat::PackagePickup()
{
	if (CurrentPort == nullptr) return;

	Package->HandlePickup(CurrentPort);
	
}

void ABoat::PackageDropoff()
{
	if (CurrentPort == nullptr) return;
	AddScore(Package->HandleDropoff(CurrentPort));
}

void ABoat::SetupShip(ShipTypes _ShipType)
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>WarShipMeshAsset(TEXT("StaticMesh'/Game/ProjectBoat/Geometry/Meshes/ships/warship/warship.warship'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>CargoShipMeshAsset(TEXT("StaticMesh'/Game/Geometry/Meshes/ships/cargo_ship/smaller_cargo_ship.smaller_cargo_ship'"));
	UStaticMesh* Asset;

	switch (_ShipType)
	{
	default:
	case CargoShip:
		Asset = CargoShipMeshAsset.Object; 
		BoatMesh->SetStaticMesh(Asset); 
		BoatMesh->SetWorldScale3D(FVector(1, 1, 1)); 
		//BoatMesh->BodyInstance.InertiaTensorScale.Set(1.4, 2, 1);
		CameraArm->TargetArmLength = 3800; 
		
		CurrentBuoyancyPoints = CargoshipBuoyancyPoints;
		CurrentPackages = CargoshipPackages;
		CurrentMissilePoints = CargoshipMissilePoints;
		CurrentTorpedoPoints = CargoshipTorpedoPoints;
		break;
	case WarShip:
		Asset = WarShipMeshAsset.Object; 
		BoatMesh->SetStaticMesh(Asset); 
		BoatMesh->SetWorldScale3D(FVector(1,1,1)); 
		BoatMesh->BodyInstance.InertiaTensorScale.Set(1.3, 1.5, 1);
		CameraArm->TargetArmLength = 2400; 

		CurrentBuoyancyPoints = WarshipBuoyancyPoints;
		CurrentPackages = WarshipPackages;
		CurrentMissilePoints = WarshipMissilePoints;
		CurrentTorpedoPoints = WarshipTorpedoPoints;
		break;
	}

	Package->SetupComponent(ShipType, CurrentPackages);
	PowerUpComponent->SetupComponent(ShipType, CurrentMissilePoints, CurrentTorpedoPoints);

	for(FVector NewPontoon : CurrentBuoyancyPoints)
		BuoyancyComponent->AddCustomPontoon(100, NewPontoon);
}

void ABoat::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HitTimer <= 0 && Health > 0)
	{
		if (Other->IsA<ABoat>())
		{
			Health -= 1;
		}
		else if (Other->IsA<AProjectileBase>())
		{
			Health -= 3;
		}
		else
		{
			DrawDebugString(GetWorld(), FVector(0, 0, 300), "Not Ship", this, FColor::Red, 1);
		}

		if (Health <= 0)
		{
			//Die
			Health = 0;
			OnDeath();
		}
	}
	HitTimer = 1.0f;
}