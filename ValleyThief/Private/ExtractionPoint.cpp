// Fill out your copyright notice in the Description page of Project Settings.


#include "ValleyThief/Public/ExtractionPoint.h"
#include "ValleyThief/ValleyThief.h"
#include "Components/BoxComponent.h"
#include "MyCharacter.h"
#include "ValleyThief/ValleyThiefGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AExtractionPoint::AExtractionPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	ExtractOverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Extraction Box"));
	ExtractOverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ExtractOverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	ExtractOverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ExtractOverlapComp->SetBoxExtent(FVector(200.f));
	RootComponent = ExtractOverlapComp;

	ExtractOverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AExtractionPoint::OverlapHandleBegin);
}

//// Called when the game starts or when spawned
//void AExtractionPoint::BeginPlay()
//{
//	Super::BeginPlay(); 
//}

void AExtractionPoint::OverlapHandleBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter* MyPawn = Cast<AMyCharacter>(OtherActor);

	if (MyPawn == nullptr)
	{
		return;
	}
	if (MyPawn->bIsCarryingObj)
	{
		AValleyThiefGameModeBase* EndMissionGameMode = Cast<AValleyThiefGameModeBase>(GetWorld()->GetAuthGameMode());
		UE_LOG(LogTemp, Log, TEXT("GameMode Casting Success"))
		if (EndMissionGameMode)
		{
			UGameplayStatics::PlaySound2D(this, PlayMissionComplete);
			EndMissionGameMode->CompleteMission(MyPawn, true);
			UE_LOG(LogTemp, Log, TEXT("Ending Mission"))
		}
		}
	else
	{
		UGameplayStatics::PlaySound2D(this, PlayExtrationFail);
	}

	UE_LOG(LogTemp, Log, TEXT("Ovelapped with the ExtractionZone"));
}
