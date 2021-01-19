// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowObjects.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class AThrowObjects : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowObjects();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category = "ThrowableProjectile")
		USphereComponent* ThrowableComp;
	UPROPERTY(VisibleAnywhere, Category = "ThrowableMovement")
		UProjectileMovementComponent* ThrowableMovementComp;
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Throw Ammo")
		int ThrowCount;
	UFUNCTION()
	bool ThrowTime();
	bool bCanThrow;
	UFUNCTION()
		void ThrowInDirection(const FVector& ThrowDirection);
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

};
