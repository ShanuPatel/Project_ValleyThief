// Fill out your copyright notice in the Description page of Project Settings.


#include "AIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "ValleyThief/ValleyThiefGameModeBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "ValleyThief/ValleyThief.h"
#include "ValleyThief/Public/Component/PlayerHealthComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"


// Sets default values
AAIGuard::AAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAIGuard::OnPawnSeen);
	PawnSensingComponent->OnHearNoise.AddDynamic(this, &AAIGuard::OnNoiseHeard);
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_TWEAPON, ECR_Ignore);

	HealthCOmp = CreateDefaultSubobject<UPlayerHealthComponent>(TEXT("AIPlayerHealthCompoenent"));
	HealthCOmp->OnHealthChanged.AddDynamic(this, &AAIGuard::OnHealthChanged);
	bIsDead = false;
	GuardState = EAISTATE::Idle;
}

void AAIGuard::SetGuardState(EAISTATE NewState)
{
	if (!bIsDead)
	{
		if (GuardState == NewState)
		{
			return;
		}
		GuardState = NewState;
		OnStateChanged(GuardState);
		if (GuardState == EAISTATE::Suspicious && GuardState != EAISTATE::Alerted)
		{
			UE_LOG(LogTemp, Warning, TEXT("HUH SOUND2"));
			UGameplayStatics::PlaySound2D(this, PlaySuspecious, 6.0f);
		}
	}
}

// Called when the game starts or when spawned
void AAIGuard::BeginPlay()
{
	Super::BeginPlay();
	if (!bIsDead)
	{
		OriginalRotation = GetActorRotation();
	if (bIsPatrolling)
	{
		UE_LOG(LogTemp, Log, TEXT(" Success beginplay"))
			MoveToNextPatrolPoint();
	}
	}	
}

void AAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}


void AAIGuard::MoveToNextPatrolPoint()
{ 
	if (!bIsDead)
	{
		if (CurrentPatrolPoint == nullptr || CurrentPatrolPoint==PatrolPointTwo)
		{
			CurrentPatrolPoint = PatrolPointOne;
		}
		else 
		{
			CurrentPatrolPoint = PatrolPointTwo;
			//GetWorldTimerManager().SetTimer(TimerHandle_WaitTime, this, &AAIGuard::MoveToNextPatrolPoint, 10.0f);
			GetWorldTimerManager().SetTimer(TimerHandle_WaitTime, this, &AAIGuard::MoveToNextPatrolPoint, 20.f);
		}
		UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatrolPoint);

		GetWorldTimerManager().SetTimer(TimerHandle_WaitTime, this, &AAIGuard::MoveToNextPatrolPoint, 50.0f);
	}
}

void AAIGuard::OnHealthChanged(UPlayerHealthComponent* HealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bIsDead)
	{
		UE_LOG(LogTemp, Log, TEXT("On HeathChange Processing"));
		bIsDead=true;
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetLifeSpan(4.0f);
		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, TEXT("Enemy Killed"));

	}
	if (Health <= 80)
	{
		SetGuardState(EAISTATE::Suspicious);
		GetWorldTimerManager().ClearTimer(TimerHandle_ResetRotation);
		GetWorldTimerManager().SetTimer(TimerHandle_ResetRotation, this, &AAIGuard::ResetRotation, 10.0f);
	}
}

void AAIGuard::OnPawnSeen(APawn* PawnSeen)
{
	if (PawnSeen == nullptr)
		{
			return;
		}
	if (!bIsDead)
		{
		DrawDebugSphere(GetWorld(), PawnSeen->GetActorLocation(), 40.f, 20.f, FColor::Magenta, false, 5.f);
		if (PawnSeen /*&& GuardState == EAISTATE::Alerted*/)
		{
			AValleyThiefGameModeBase* EndMissionGameMode = Cast<AValleyThiefGameModeBase>(GetWorld()->GetAuthGameMode());
			UE_LOG(LogTemp, Log, TEXT("GameMode Casting Success"))
				if (EndMissionGameMode)
				{
					EndMissionGameMode->CompleteMission(PawnSeen, false);
					UE_LOG(LogTemp, Log, TEXT("Ending Mission"))
						UGameplayStatics::PlaySound2D(this, PlayGameOver);
						SetLifeSpan(.4f);
				}
		}
		SetGuardState(EAISTATE::Alerted);
		if (Controller)
		{
			Controller->StopMovement();
		}
	}
}

void AAIGuard::OnNoiseHeard(APawn* InstigatorOnNoise, const FVector& Location, float Volume)
{
	if (GuardState == EAISTATE::Alerted)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("HUH SOUND1"));	//DrawDebugSphere(GetWorld(), Location, 40.f, 12.0f, FColor::Purple, false, 5.f);
	if (!bIsDead )
	{
		FVector Direction = Location - GetActorLocation();
		Direction.Normalize();
		FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
		NewLookAt.Pitch = 0.0f;
		NewLookAt.Roll = 0.0f;
		SetActorRotation(NewLookAt);
		GetWorldTimerManager().ClearTimer(TimerHandle_ResetRotation);
		GetWorldTimerManager().SetTimer(TimerHandle_ResetRotation, this, &AAIGuard::ResetRotation, 5.0f);
		SetGuardState(EAISTATE::Suspicious);
		UE_LOG(LogTemp, Warning, TEXT("Try Running Once"));
		if (GuardState==EAISTATE::Suspicious && GuardState!=EAISTATE::Alerted)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, TEXT("Caution"));
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), Location);
		}
	}
}

void AAIGuard::ResetRotation()
{
	if (!bIsDead)
	{
		if (GuardState == EAISTATE::Alerted)
		{
			return;
		}
		SetActorRotation(OriginalRotation);
		SetGuardState(EAISTATE::Idle);
	}
	SetGuardState(EAISTATE::Idle);
	if (bIsPatrolling)
	{
		MoveToNextPatrolPoint();
	}
}



// Called every frame

// Called to bind functionality to input
//void AAIGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

