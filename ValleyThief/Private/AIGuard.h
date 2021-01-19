// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIGuard.generated.h"

class UPawnSensingComponent;
class UPlayerHealthComponent;

UENUM(BlueprintType)
enum class EAISTATE : uint8
{
 Idle,
 Suspicious,
 Alerted
};

UCLASS()
class AAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Components")
		USkeletalMeshComponent* AIGuard;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Components")
		UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
		bool bIsPatrolling;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (EditCondition = "bIsPatrolling"))
		AActor* PatrolPointOne;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (EditCondition = "bIsPatrolling"))
		AActor* PatrolPointTwo;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (EditCondition = "bIsPatrolling"))
		AActor* CurrentPatrolPoint;
	UFUNCTION(BlueprintCallable, Category = "AI Comps")
	void MoveToNextPatrolPoint();
	FTimerHandle TimerHandle_WaitTime;

	UPlayerHealthComponent* HealthCOmp;
	UFUNCTION()
		void OnHealthChanged(UPlayerHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "GaurdPlayer")
	bool bIsDead;
	UFUNCTION()
		void OnPawnSeen(APawn* PawnSeen);
	UFUNCTION()
		void OnNoiseHeard(APawn* InstigatorOnNoise, const FVector& Location, float Volume);

	UPROPERTY()
		FRotator OriginalRotation;
	UPROPERTY()
		FTimerHandle TimerHandle_ResetRotation;
	UFUNCTION()
		void ResetRotation();
	UPROPERTY(EditDefaultsOnly, Category = "SoundFX")
		USoundBase* PlaySuspecious;
	UPROPERTY(EditDefaultsOnly, Category = "SoundFX")
		USoundBase* PlayGameOver;

	EAISTATE GuardState;
	void SetGuardState(EAISTATE NewState);
	UFUNCTION(BlueprintImplementableEvent, Category= "AI Components")
	void OnStateChanged(EAISTATE NewState);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
