// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "ValleyThief/ValleyThief.h"
#include "Animation/AnimSequence.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"
// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	GunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	RootComponent = GunMeshComp;
	FireAnimation = nullptr;
	SlideLockAnimation = nullptr;
	TotalAmmo = 42;
	PistolAmmo = 7;
	PistolReloadAnimation = nullptr;
}

// Called when the game starts or when spawned
//void AWeaponBase::BeginPlay()
//{
//	Super::BeginPlay();
//
//}

// Called every frame

bool AWeaponBase::Fire()
{
	if (GunMeshComp && FireAnimation)
	{
		if (PistolAmmo > 0 && TotalAmmo>=0)
		{
			 if (PistolAmmo == 1)
			{
				 GEngine->AddOnScreenDebugMessage(-1, 4.0, FColor::Orange, TEXT("Press 'R' to Reload"));
				if(SlideLockAnimation)
				GunMeshComp->PlayAnimation(SlideLockAnimation, false);
			}
			else
			{
				GunMeshComp->PlayAnimation(FireAnimation, false);
			}
			--PistolAmmo;
			return true;
		}

	}
	return false;
}

bool AWeaponBase::IsSlideLocked()
{
	return PistolAmmo == 0;
}


bool AWeaponBase::Reload()
{
	if (PistolAmmo <= 0  && PistolReloadAnimation)
	{
		if (PistolReloadAnimation)
		{
			GunMeshComp->PlayAnimation(PistolReloadAnimation, false);
		}
		/*else if(PistolAmmo < 7 && PistolAmmo!=7)
		{
			GunMeshComp->PlayAnimation(PistolReloadAnimation, false);
		}*/

	}

	PistolAmmo = 7;
	TotalAmmo =TotalAmmo - PistolAmmo;

	return true;
}

