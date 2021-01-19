// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class UAnimSequence;
UCLASS()
class AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBase();

private:
	UPROPERTY(EditAnywhere, Category = "FireAnimation")
		UAnimSequence* FireAnimation;

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Mesh")
		USkeletalMeshComponent* GunMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReloadAnimations")
		UAnimSequence* SlideLockAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReloadAnimations")
		UAnimSequence* PistolReloadAnimation;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int PistolAmmo;*/
public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int PistolAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int TotalAmmo;
	bool Fire();
	bool Reload();
	bool IsSlideLocked();
};
