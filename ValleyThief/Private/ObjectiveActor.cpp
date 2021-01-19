// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectiveActor.h"
#include "ValleyThief/ValleyThief.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"

// Sets default values
AObjectiveActor::AObjectiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	ObjMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Objective Mesh"));
	RootComponent = ObjMeshComp;
	ObjMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ObjSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Objective Sphere Component"));
	ObjSphereComp->SetupAttachment(ObjMeshComp);
	ObjSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ObjSphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	ObjSphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

// Called when the game starts or when spawned
void AObjectiveActor::BeginPlay()
{
	Super::BeginPlay();
	PlayEffect();
}

void AObjectiveActor::PlayEffect()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ObjectiveEffect, GetActorLocation());
}

// Called every frame
void AObjectiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

		PlayEffect();
		UGameplayStatics::PlaySound2D(this, PlayObjectivePicked);
		UE_LOG(LogTemp, Warning, TEXT("Sphere Overlaped"));
		AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);
		if(MyCharacter)
		{
			MyCharacter->bIsCarryingObj= true;
			Destroy();
			UE_LOG(LogTemp, Warning, TEXT("Is Carrying Objective"))
		}
}

