// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Port.h"
#include "BoatType.h"
#include "PackageType.h"
#include "HUDWidget.h"
#include "BoatPackageComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTBOAT_API UBoatPackageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBoatPackageComponent();
	void SetupComponent(ShipTypes Type, TArray<FVector> _PackageLocations);
	void SetHUD(UHUDWidget* _CurrentHUD) { CurrentHUD = _CurrentHUD; if (CurrentHUD != nullptr) CurrentHUD->SetPackages(Packages); };
	
	bool CheckPort(APort* CurrentPort);
	void HandlePickup(APort* _OriginPort, bool HasExactDestination = false);
	int HandleDropoff(APort* CurrentPort, bool HasExactDestination = false);
	
	AActor* SpawnPackageActor(FVector Location);
	UFUNCTION(Server, Reliable) void Server_SpawnPackageActor(FPackages Package);
	UFUNCTION(Server, Reliable) void Server_DestroyPackageActor(int Slot);

	//TODO get rid of this shit
	UFUNCTION(BlueprintCallable) int GetPackageCount() { return Packages.Num(); };
	UFUNCTION(BlueprintCallable) FString GetPackageType(int Slot = 0);
	UFUNCTION(BlueprintCallable) FString GetPackageOrigin(int Slot = 0);
	UFUNCTION(BlueprintCallable) FString GetPackageDestination(int Slot = 0);
	UFUNCTION(BlueprintCallable) int32 GetPackageValue(int Slot = 0) { if (Packages.Num() <= Slot) return 0; return Packages[Slot].CurrentValue; };
	UFUNCTION(BlueprintCallable) float GetPackageExplodesIn(int Slot = 0) { if (Packages.Num() <= Slot) return 0; return Packages[Slot].ExplodesIn; };
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TArray<FPackages> Packages;
	int MaxPackageCount = 0;

	TArray<FVector> PackageLocations;

	UPROPERTY(EditAnywhere)	int32 RegularPackageValue = 1000;
	UPROPERTY(EditAnywhere)	float RegularPackageValueDecreaseTime = 10;
	UPROPERTY(EditAnywhere)	int32 BombPackageValue = 2000;
	UPROPERTY(EditAnywhere)	float BombPackageValueDecreaseTime = 5;
	UPROPERTY(EditAnywhere)	float BombPackageExplosionTime = 100;
	UPROPERTY(EditAnywhere)	int32 FragilePackageValue = 2000;
	UPROPERTY(EditAnywhere)	float FragilePackageValueDecreaseTime = 2;

	int Width = 3;
	int Length = 3;
	int Height = 2;

	UHUDWidget* CurrentHUD = nullptr;
};
