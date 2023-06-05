// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/PostProcessComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "BoatMovementComponent.h"
#include "BoatReplicationComponent.h"
#include "BoatPackageComponent.h"
#include "BoatPowerUpComponent.h"
#include "BuoyancyComponent.h"
#include "Port.h"
#include "ProjectBoatGameInstance.h"
#include "BoatType.h"
#include "Boat.generated.h"

UCLASS()
class PROJECTBOAT_API ABoat : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABoat();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION() void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable) void RemovePackage() { HasPackage = false; };
	UFUNCTION(BlueprintCallable) bool PackageExists() { return HasPackage; };

	UFUNCTION(BlueprintCallable) int32 GetScore() { return Score; };
	UFUNCTION(BlueprintCallable) void AddScore(int32 _Score) { Score += _Score; if (ReplicationComponent == nullptr) return; ReplicationComponent->OnScoreChanged(Score); };
	UFUNCTION() void SetScore(int32 _Score) { Score = _Score; };

	UFUNCTION(BlueprintCallable) ShipTypes GetShipType() { return ShipType; };
	void ChangeShipType(TEnumAsByte<ShipTypes> _ShipType) { ShipType = _ShipType; SetupShip(ShipType); if (ReplicationComponent == nullptr) return; ReplicationComponent->OnShipTypeChanged(ShipType); };
	UFUNCTION(BlueprintCallable) void SetShipType(TEnumAsByte<ShipTypes> _ShipType) { ShipType = _ShipType; SetupShip(ShipType); };

	UFUNCTION(BlueprintCallable) int32 GetHealth() { return Health; };
	UFUNCTION(BlueprintCallable) void SetHealth(int32 _Health) { Health = _Health; };

	UPROPERTY(EditAnywhere, BlueprintReadWrite) USpringArmComponent* CameraArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* BoatMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UBoatMovementComponent* MovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UBuoyancyComponent* BuoyancyComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UBoatPackageComponent* Package;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UBoatPowerUpComponent* PowerUpComponent;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite) UArrowComponent* BuoyancyPointTopLeft;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite) UArrowComponent* BuoyancyPointTopRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) USpringArmComponent* MenuCameraArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UCameraComponent* MenuCamera;

	UFUNCTION(BlueprintCallable) void Die(FVector RespawnLocation, FRotator RespawnRotation);
	UFUNCTION(BlueprintCallable)void SetupShip(ShipTypes _ShipType);

	UFUNCTION(BlueprintImplementableEvent, Category = "BaseCharacter") void OnDeath();
	
	UPROPERTY(EditAnywhere, Category = "Buoyancy", Meta = (MakeEditWidget = true)) TArray<FVector> CurrentBuoyancyPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buoyancy") TArray<FVector> CargoshipBuoyancyPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buoyancy") TArray<FVector> WarshipBuoyancyPoints;

	UPROPERTY(EditAnywhere, Category = "Missile", Meta = (MakeEditWidget = true)) TArray<FVector> CurrentMissilePoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile") TArray<FVector> CargoshipMissilePoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile") TArray<FVector> WarshipMissilePoints;
	
	UPROPERTY(EditAnywhere, Category = "Torpedo", Meta = (MakeEditWidget = true)) TArray<FVector> CurrentTorpedoPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torpedo") TArray<FVector> CargoshipTorpedoPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torpedo") TArray<FVector> WarshipTorpedoPoints;

	UPROPERTY(EditAnywhere, Category = "Packages", Meta = (MakeEditWidget = true)) TArray<FVector> CurrentPackages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Packages") TArray<FVector> CargoshipPackages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Packages") TArray<FVector> WarshipPackages;

	UHUDWidget *CurrentHUD = nullptr;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Respawn(FVector RespawnLocation, FRotator RespawnRotation);
	

private:
	//UPROPERTY(ReplicatedUsing = OnRepTransform) FTransform RepTransform;
	//UFUNCTION() void OnRepTransform();

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Jump();
	void UseBoost();
	void StopBoost();

	void UsePrimaryPowerUp();
	void UseSecondaryPowerUp();

	void PackagePickup();
	void PackageDropoff();
	bool HasPackage = false;

	int32 Score = 0;

	int32 MaxHealth = 10;
	int32 Health = MaxHealth;
	bool IsRespawning = false;

	float HitTimer = 0.0f;
	UFUNCTION() virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	APort* CurrentPort = nullptr;

	UPROPERTY(EditAnywhere) UMaterial* BoostPostProcessMaterial = nullptr;
	UPROPERTY() UMaterialInstanceDynamic *BoostPostProcess = nullptr;

	UPROPERTY(EditAnywhere) TEnumAsByte<ShipTypes> ShipType = ShipTypes::WarShip;

	UPROPERTY(EditAnywhere) UPostProcessComponent* PostProcessComponent;
	UPROPERTY(EditAnywhere) UBoatReplicationComponent* ReplicationComponent;
	
	
};
