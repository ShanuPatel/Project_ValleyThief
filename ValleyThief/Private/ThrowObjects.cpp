// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowObjects.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
AThrowObjects::AThrowObjects()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick =false;

	ThrowableComp = CreateDefaultSubobject<USphereComponent>(TEXT("THrowAbles Compoenents"));
	ThrowableComp->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	ThrowableComp->OnComponentHit.AddDynamic(this, &AThrowObjects::OnHit);
	ThrowableComp->InitSphereRadius(20.f);
	RootComponent = ThrowableComp;

	ThrowableMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ThrowableMovementCompoenent"));
	ThrowableMovementComp->SetUpdatedComponent(ThrowableComp);
	ThrowableMovementComp->InitialSpeed = 3000.f;
	ThrowableMovementComp->MaxSpeed = 3000.f;
	ThrowableMovementComp->bRotationFollowsVelocity = true;
	ThrowableMovementComp->bShouldBounce = true;
	ThrowableMovementComp->Bounciness = 0.1f;
	InitialLifeSpan = 3.0f;
	bCanThrow = false;
	ThrowCount = 2;
}

// Called when the game starts or when spawned
void AThrowObjects::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AThrowObjects::ThrowTime()
{
	if (ThrowableComp)
	{
		if (ThrowCount > 0)
		{
			bCanThrow=true;
		}
		--ThrowCount;
		return true;
	}
	return false;
}

void AThrowObjects::ThrowInDirection(const FVector& ThrowDirection)
{
	ThrowableMovementComp->Velocity = ThrowDirection * ThrowableMovementComp->InitialSpeed;
}

void AThrowObjects::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComponent != NULL) && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}
	UE_LOG(LogTemp, Log, TEXT("Projectile Hit Contact"));
	MakeNoise(1.0f, Instigator);
	Destroy();
}

//void AThrowObjects::OnThrown()
//{
//}

// Called every frame
//void AThrowObjects::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

