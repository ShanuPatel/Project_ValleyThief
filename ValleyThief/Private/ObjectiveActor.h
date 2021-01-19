// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectiveActor.generated.h"

class USphereComponent;
UCLASS()
class AObjectiveActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AObjectiveActor();

private:
	void PlayEffect();
	UPROPERTY(EditDefaultsOnly, Category = "SoundFX")
		USoundBase* PlayObjectivePicked;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "objective Comp")
		UStaticMeshComponent* ObjMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Objective Comp")
		USphereComponent* ObjSphereComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objective Effect")
		UParticleSystem* ObjectiveEffect;
public:
	// Called every frame

	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor)override;

};
