// Fill out your copyright notice in the Description page of Project Settings.


#include "BoatPowerUpComponent.h"

// Sets default values for this component's properties
UBoatPowerUpComponent::UBoatPowerUpComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UBoatPowerUpComponent::SetupComponent(ShipTypes _ShipType, TArray<FVector> _MissilePoints, TArray<FVector> _TorpedoPoints)
{
	ShipType = _ShipType;
	if (PowerUps.Num() != 0)
	{
		PowerUps.Empty();
	}
	
	MissilePoints = _MissilePoints;
	TorpedoPoints = _TorpedoPoints;
	for (int i = 0; i < FixPowerUps; i++)
	{
		// TODO Setup fix powerups
		// AddPowerUp(Type);
	}
}

// Called when the game starts
void UBoatPowerUpComponent::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent = GetOwner()->FindComponentByClass<UBoatMovementComponent>();
	
}


// Called every frame
void UBoatPowerUpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (int i = 0; i < PowerUps.Num(); i++)
	{
		if (PowerUps[i].CurrentCooldown != 0)
		{
			if (PowerUps[i].CurrentCooldown < DeltaTime)
			{
				PowerUps[i].CurrentCooldown = 0;
			}
			else PowerUps[i].CurrentCooldown -= DeltaTime;
		}
	}
}

void UBoatPowerUpComponent::PickupRandom()
{
	if (PowerUps.Num() < MaxPowerUps)
	{
		TEnumAsByte<PowerUpTypes> PowerUpType;
		int Count;
		int32 TypeChance = FMath::RandHelper(100);

		if (TypeChance < 20)
		{
			PowerUpType = Missile;
			Count = FMath::RandHelper(2) + 1;
		}
		else if (TypeChance < 40)
		{
			PowerUpType = Torpedo;
			Count = FMath::RandHelper(3) + 1;
		}
		else if (TypeChance < 60)
		{
			PowerUpType = Smoke;
			Count = FMath::RandHelper(3) + 1;
		}
		else if (TypeChance < 80)
		{
			PowerUpType = Jump;
			Count = FMath::RandHelper(1) + 1;
		}
		else if (TypeChance < 100)
		{
			PowerUpType = Dash;
			Count = FMath::RandHelper(3) + 1;
		}
		AddPowerUp(PowerUpType, Count);
	}
}

void UBoatPowerUpComponent::AddPowerUp(PowerUpTypes Type, int Count)
{
	FPowerUps PowerUp;
	PowerUp.PowerUpType = Type;
	PowerUp.Count = Count;
	PowerUp.CanSpawn = true;
	switch (Type)
	{
	case Missile:
		PowerUp.Cooldown = 5;
		PowerUp.IsActorNeeded = true;
		break;
	case Torpedo:
		PowerUp.Cooldown = 3;
		PowerUp.IsActorNeeded = true;
		break;
	case Smoke:
		PowerUp.Cooldown = 3;
		PowerUp.IsActorNeeded = true;
		break;
	case Jump:
		PowerUp.Cooldown = 7;
		PowerUp.IsActorNeeded = false;
		break;
	case Dash:
		PowerUp.Cooldown = 5;
		PowerUp.IsActorNeeded = false;
		break;
	default:
		break;
	}
	PowerUp.CurrentCooldown = 0;


	PowerUps.Add(PowerUp);

	if (CurrentHUD != nullptr) CurrentHUD->SetPowerUps(PowerUps);
}

void UBoatPowerUpComponent::UsePowerUp(int Slot, FTransform CameraTransform)
{
	if (PowerUps.Num() <= Slot) return;

	if (PowerUps[Slot].CurrentCooldown == 0 && PowerUps[Slot].CanSpawn)
	{
		PowerUps[Slot].Count -= 1;
		// Spawn actor if needed
		if (PowerUps[Slot].IsActorNeeded)
		{
			FVector SpawnLocation;
			FRotator SpawnRotation = GetOwner()->GetActorRotation();
			if (PowerUps[Slot].PowerUpType == Missile)
			{
				int Point = FMath::RandHelper(MissilePoints.Num());
				SpawnLocation = MissilePoints[Point];
			}
			else if (PowerUps[Slot].PowerUpType == Torpedo)
			{
				float ClosestDP = -1;
				for (FVector Point : TorpedoPoints)
				{
					float CurrentDP = FVector::DotProduct(GetOwner()->GetActorRotation().RotateVector(Point), CameraTransform.GetLocation());
					if (CurrentDP > ClosestDP)
					{
						ClosestDP = CurrentDP;
						SpawnLocation = Point;
					}
				}
				SpawnRotation = CameraTransform.GetRotation().Rotator();
			}
			else SpawnLocation = FVector::ZeroVector;

			SpawnRotation.Pitch = 0;
			SpawnRotation.Roll = 0;

			if (GetOwner()->HasAuthority())
			{
				PowerUps[Slot].PowerUpActor = SpawnPowerUpActor(PowerUps[Slot], SpawnLocation, SpawnRotation); // TODO replace with normal spawn loc
			}
			else
			{
				Server_SpawnPowerUpActor(PowerUps[Slot], SpawnLocation, SpawnRotation);
			}
		}

		// Handle PowerUp
		switch (PowerUps[Slot].PowerUpType)
		{
		case Missile:
			PowerUps[Slot].CanSpawn = false;
			break;
		case Torpedo:
			PowerUps[Slot].CurrentCooldown = PowerUps[Slot].Cooldown;
			break;
		case Smoke:
			PowerUps[Slot].CurrentCooldown = PowerUps[Slot].Cooldown;
			break;
		case Jump:
			if (MovementComponent == nullptr) return;
			MovementComponent->Jump();
			PowerUps[Slot].CurrentCooldown = PowerUps[Slot].Cooldown;
			break;
		case Dash:
			if (MovementComponent == nullptr) return;
			MovementComponent->Dash();
			PowerUps[Slot].CurrentCooldown = PowerUps[Slot].Cooldown;
			break;
		default:
			break;
		}

		// Remove PowerUp from inventory
		if (PowerUps[Slot].Count == 0)
		{
			PowerUps.RemoveAt(Slot);
		}

		if (CurrentHUD != nullptr) CurrentHUD->SetPowerUps(PowerUps);
	}
}

AActor* UBoatPowerUpComponent::SpawnPowerUpActor(FPowerUps PowerUp, FVector Location, FRotator Rotation)
{
	FPowerUps New = PowerUp;
	UClass* PowerUpBlueprintClass;
	switch (PowerUp.PowerUpType)
	{
	case Missile:
		PowerUpBlueprintClass = StaticLoadClass(UObject::StaticClass(), NULL, TEXT("/Game/ProjectBoat/Characters/BP_TestBullet.BP_TestBullet_C"), NULL, LOAD_None, NULL);
		break;
	case Torpedo:
		PowerUpBlueprintClass = StaticLoadClass(UObject::StaticClass(), NULL, TEXT("/Game/ProjectBoat/Characters/BP_TestTorpedo.BP_TestTorpedo_C"), NULL, LOAD_None, NULL);
		break;
	case Smoke:
		return nullptr;
	default:
		return nullptr;
	}
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.Owner = GetOwner();

	AActor* NewPowerUp = GetOwner()->GetWorld()->SpawnActor<AActor>(PowerUpBlueprintClass,
		GetOwner()->GetActorLocation() + GetOwner()->GetActorRotation().RotateVector(Location), Rotation, SpawnInfo);

	New.PowerUpActor = NewPowerUp;
	if (!GetOwner<APawn>()->IsLocallyControlled())
		PowerUps.Add(New);
	return NewPowerUp;
}

void UBoatPowerUpComponent::Server_SpawnPowerUpActor_Implementation(FPowerUps PowerUp, FVector Location, FRotator Rotation)
{
	SpawnPowerUpActor(PowerUp, Location, Rotation); // TODO replace with normal spawn loc
}

void UBoatPowerUpComponent::OnPowerUpActorDestroyed(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("Detected Destroyed PowerUp"));
	for (int i = 0; i < PowerUps.Num(); i++)
	{
		if (PowerUps[i].PowerUpActor == Actor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Actor"));
			PowerUps[i].PowerUpActor = nullptr;
			if (GetOwner<APawn>()->IsLocallyControlled())
			{
				UE_LOG(LogTemp, Warning, TEXT("Found Destroyed PowerUp"));
				PowerUps[i].CanSpawn = true;
				PowerUps[i].CurrentCooldown = PowerUps[i].Cooldown;
				UE_LOG(LogTemp, Warning, TEXT("Successfully Reset PowerUp"));
			}
			else
			{
				Client_RemovePowerUp(PowerUps[i]);
				PowerUps.RemoveAt(i);
			}
			return;
		}
	}
}

void UBoatPowerUpComponent::RemovePowerUp(FPowerUps PowerUp)
{
	for (int i = 0; i < PowerUps.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reseting Destroyed PowerUp..."));
		if (PowerUps[i].PowerUpType == PowerUp.PowerUpType &&
			PowerUps[i].Count == PowerUp.Count)
		{
			PowerUps[i].CanSpawn = true;
			PowerUps[i].CurrentCooldown = PowerUps[i].Cooldown;
			UE_LOG(LogTemp, Warning, TEXT("Successfully Reset PowerUp"));
			return;
		}
	}
}

void UBoatPowerUpComponent::Client_RemovePowerUp_Implementation(FPowerUps PowerUp) 
{ 
	UE_LOG(LogTemp, Warning, TEXT("Client Recieved Destroyed PowerUp"));
	RemovePowerUp(PowerUp);
}