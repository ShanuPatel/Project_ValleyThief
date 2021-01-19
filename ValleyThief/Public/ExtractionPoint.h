// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExtractionPoint.generated.h"

class UBoxComponent;
UCLASS()
class VALLEYTHIEF_API AExtractionPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExtractionPoint();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, Category="Extration Zone")
	UBoxComponent* ExtractOverlapComp;
	UFUNCTION()
	void OverlapHandleBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category = "SoundFX")
		USoundBase* PlayExtrationFail;
	UPROPERTY(EditDefaultsOnly, Category = "SoundFX")
		USoundBase* PlayMissionComplete;
public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
