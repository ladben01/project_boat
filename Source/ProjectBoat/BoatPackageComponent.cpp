// Fill out your copyright notice in the Description page of Project Settings.


#include "BoatPackageComponent.h"

// Sets default values for this component's properties
UBoatPackageComponent::UBoatPackageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UBoatPackageComponent::SetupComponent(ShipTypes Type, TArray<FVector> _PackageLocations)
{
	//Packages.Empty();
	switch (Type)
	{
	case CargoShip:
		MaxPackageCount = 2;
		Width = 5;
		Length = 5;
		Height = 4;
		break;
	case WarShip:
		MaxPackageCount = 1;
		Width = 3;
		Length = 3;
		Height = 2;
		break;
	}

	PackageLocations = _PackageLocations;
}

bool UBoatPackageComponent::CheckPort(APort* CurrentPort)
{
	for (FPackages Package : Packages)
	{
		if (Package.DestinationPort != nullptr)
		{
			// megnézzük hogy jó helyen van-e
			return true;
		}
		else if (CurrentPort != Package.OriginPort)
		{
			return true;
		}
	}
	return false;
}

// Called when the game starts
void UBoatPackageComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UBoatPackageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (int Slot = 0; Slot < Packages.Num(); Slot++)
	{
		if (Packages[Slot].ExplodesIn != -1)
		{
			Packages[Slot].ExplodesIn -= DeltaTime;
			if (CurrentHUD != nullptr)
				CurrentHUD->SetPackageExplosion(Slot, Packages[Slot].ExplodesIn);
		}

		if (Packages[Slot].ValueDecreasesIn > 0)
			Packages[Slot].ValueDecreasesIn -= DeltaTime;
		else
		{
			if (Packages[Slot].CurrentValue > 0)
			{
				Packages[Slot].CurrentValue--;
				Packages[Slot].ValueDecreasesIn = Packages[Slot].ValueDecreaseTime;
				if (CurrentHUD != nullptr)
					CurrentHUD->SetPackageValue(Slot, Packages[Slot].CurrentValue);
			}
		}
	}
	
}

void UBoatPackageComponent::HandlePickup(APort* _OriginPort, bool HasExactDestination)
{
	if (Packages.Num() < MaxPackageCount)
	{
		FPackages Package;
		
		Package.OriginPort = _OriginPort;
		if (HasExactDestination)
		{
			// Set the destination to a non null port
		}
		else
		{
			Package.DestinationPort = nullptr;
		}
		int32 TypeChance = FMath::RandHelper(100);

		if (TypeChance <= 60)			//Regular
		{
			Package.PackageType = Regular;
			Package.CurrentValue = RegularPackageValue;

			Package.ValueDecreaseTime = RegularPackageValueDecreaseTime; //x sec alatt decreasel 1 pontot
			Package.ValueDecreasesIn = Package.ValueDecreaseTime;
			Package.ExplodesIn = -1;
		}
		else if (TypeChance <= 80)		//Bomb
		{
			Package.PackageType = Bomb;
			Package.CurrentValue = BombPackageValue;

			Package.ValueDecreaseTime = BombPackageValueDecreaseTime; //x sec alatt decreasel 1 pontot
			Package.ValueDecreasesIn = Package.ValueDecreaseTime;
			Package.ExplodesIn = BombPackageExplosionTime; //x sec alatt robban fel
		}
		else							//Fragile
		{
			Package.PackageType = Fragile;
			Package.CurrentValue = FragilePackageValue;

			Package.ValueDecreaseTime = FragilePackageValueDecreaseTime; //x sec alatt decreasel 1 pontot
			Package.ValueDecreasesIn = Package.ValueDecreaseTime;
			Package.ExplodesIn = -1;
		}

		if (Packages.Num() == 0)
		{
			Package.Location = PackageLocations[0];
		}
		else
		{
			for (FVector V : PackageLocations)
			{
				bool CanSpawnThere = true;
				for (FPackages P : Packages)
				{
					if (P.Location == V) CanSpawnThere = false;
				}
				if (CanSpawnThere)
				{
					Package.Location = V;
					break;
				}
			}
		}
		Packages.Add(Package);

		if (GetOwner()->HasAuthority())
		{
			Packages.Last().Actor = SpawnPackageActor(Packages.Last().Location);
		}
		else
		{
			Server_SpawnPackageActor(Package);
		}

		if (CurrentHUD != nullptr)
			CurrentHUD->SetPackages(Packages);
	}
}

int UBoatPackageComponent::HandleDropoff(APort* CurrentPort, bool HasExactDestination)
{
	if (Packages.Num() > 0)
	{
		for (int Slot = 0; Slot < Packages.Num(); Slot++)
		{
			if (GetPackageDestination() == "Any Port")
			{
				if (CheckPort(CurrentPort))
				{
					int Value = Packages[Slot].CurrentValue;
					
					if (GetOwner()->HasAuthority())
					{
						if (Packages[Slot].Actor != nullptr)
						{
							Packages[Slot].Actor->Destroy();
							Packages.RemoveAt(Slot);
						}
						else UE_LOG(LogTemp, Error, TEXT("No package to destroy"));
					}
					else
					{
						UE_LOG(LogTemp,Warning,TEXT("Client tries to destroy package"));
						Server_DestroyPackageActor(Slot);
						Packages.RemoveAt(Slot);
					}
					if (CurrentHUD != nullptr)
						CurrentHUD->SetPackages(Packages);
					return Value;
					//DrawDebugString(GetWorld(), FVector(0, 0, 300), TEXT("Package dropped off!"), this, FColor::Green, 3);
				}
				//else DrawDebugString(GetWorld(), FVector(0, 0, 300), TEXT("Can't drop off package!"), this, FColor::Red, 3);
			}
			else if (true)
			{
				// jó helyre ment a csomag
			}
			else
			{
				// nem jó hely
			}
		}
	}
	return 0;
}

AActor* UBoatPackageComponent::SpawnPackageActor(FVector Location)
{
	UClass* MyItemBlueprintClass = StaticLoadClass(UObject::StaticClass(), NULL, TEXT("/Game/ProjectBoat/Characters/BP_Packages.BP_Packages_C"), NULL, LOAD_None, NULL);
	FActorSpawnParameters SpawnInfo;

	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor *NewPackage = GetOwner()->GetWorld()->SpawnActor<AActor>(MyItemBlueprintClass,
		GetOwner()->GetActorLocation() + GetOwner()->GetActorRotation().RotateVector(Location), GetOwner()->GetActorRotation(), SpawnInfo);
	if(NewPackage != nullptr)
		NewPackage->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
	return NewPackage;
}

void UBoatPackageComponent::Server_SpawnPackageActor_Implementation(FPackages Package)
{
	Package.Actor = SpawnPackageActor(Package.Location);
	Packages.Add(Package);
}

void UBoatPackageComponent::Server_DestroyPackageActor_Implementation(int Slot)
{
	bool Success;
	if(Packages[Slot].Actor != nullptr) Success = Packages[Slot].Actor->Destroy();
	Packages.RemoveAt(Slot);

	if (Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("The package was destroyed"));
	}
	else
		UE_LOG(LogTemp, Error, TEXT("The package was not destroyed"));
}

FString UBoatPackageComponent::GetPackageType(int Slot)
{
	if (Packages.Num() <= Slot) return "null";

	switch (Packages[Slot].PackageType)
	{
	default:
	case Regular:
		return "Regular Goods";
	case Bomb:
		return "Explosives";
	case Fragile:
		return "Fragile";
	}
}

FString UBoatPackageComponent::GetPackageOrigin(int Slot)
{
	if (Packages.Num() <= Slot) return "null";
	if (Packages[Slot].OriginPort == nullptr) return "null";
	else return Packages[Slot].OriginPort->GetPortName();
}

FString UBoatPackageComponent::GetPackageDestination(int Slot)
{
	if (Packages.Num() <= Slot) return "null";
	if (Packages[Slot].DestinationPort == nullptr) return "Any Port";
	else return Packages[Slot].DestinationPort->GetPortName();
}