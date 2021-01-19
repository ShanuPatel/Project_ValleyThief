// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UCameraComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UPawnNoiseEmitterComponent;
UCLASS()
class AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
		void MoveForward(float value);
private:
	UFUNCTION()
		void MoveRight(float value);
	UFUNCTION()
		void BeginCrouch();
	UFUNCTION()
		void StopCrouch();
	UFUNCTION()
		void JumpStart();
	UFUNCTION()
		void JumpEnds();
	UFUNCTION()
		void Sprinting();
	UFUNCTION()
		void SprintingStops();
	bool bCanFire;
	bool bIsSprinting;
	void OnThrown();
	void OnFire();
	/*void OnFireStop();*/
	UFUNCTION()
		void StartADS();
	UFUNCTION()
		void StopADS();

	bool bIsADS;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeaponBase>WeaponClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AThrowObjects>ThrowableClass;
	UPROPERTY(EditDefaultsOnly, Category = "WeaponComponent")
		TSubclassOf<UDamageType> DamageTypes;

	FTimerHandle TADSHandler;
	void ADSCameraFov();

	void OnReload();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera Component")
		UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, blueprintReadWrite, Category = MeshComponent, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GunMeshCompoenent, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* GunMesh;
	UPROPERTY(VisibleDefaultsOnly, Category = "Gun Component")
		USceneComponent* MuzzleLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Component")
		FVector GunOffset;
	UPROPERTY(EditDefaultsOnly, Category = "SoundFX")
		USoundBase* PlayEmptyMag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Animation")
		UAnimMontage* FireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Throwables")
		FVector ThrowOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload Animation")
		UAnimMontage* ReloadAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Component")
		UPawnNoiseEmitterComponent* PawnMakingNoiseComp;
	UFUNCTION(BlueprintCallable, category="Sprinting")
		bool IsPlayerSprinting();
	UPROPERTY(BlueprintReadWrite, Category="Objective")
	bool bIsCarryingObj;

protected:

	/*UPROPERTY()
		FTimerHandle Handle_ThrowWait;*/
	class AThrowObjects* throwabless;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Weapon")
	class AWeaponBase* Weapon;

	UFUNCTION(BluePrintCallable, Category = "AimDownSight")
	bool IsPlayerADS();
};
