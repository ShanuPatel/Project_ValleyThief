// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "ValleyThief/ValleyThief.h"
#include "WeaponBase.h"
#include "ThrowObjects.h"
#include "ValleyThief/Private/ThrowObjects.h"
#include "Engine.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/PawnNoiseEmitterComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FpsCameraComponent"));
	CameraComp->SetupAttachment(GetCapsuleComponent());
	CameraComp->SetRelativeLocation(FVector(-40.0f, 2.0f, 64.f));
	CameraComp->bUsePawnControlRotation = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh component"));
	MeshComp->SetupAttachment(CameraComp);
	MeshComp->SetOnlyOwnerSee(true);
	MeshComp->bCastDynamicShadow = false;
	MeshComp->CastShadow = false;
	GetMesh()->SetOwnerNoSee(true);

	PawnMakingNoiseComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT(" Pawn Noise Maker Component"));

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh Component"));
	GunMesh->SetupAttachment(RootComponent);

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
	MuzzleLocation->SetupAttachment(GunMesh);
	MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	GunOffset = FVector(100.0f, 0.0f, 10.0f);
	ThrowOffset= FVector(100.0f, 0.0f, 10.0f);
	/*bCanFire = false;*/
	bIsSprinting = false;
	bIsADS = false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->bShowMouseCursor = false;
	}
	Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);
	if (Weapon)
	{
		Weapon->AttachToComponent(MeshComp, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
	//GunMesh->AttachToComponent(MeshComp, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("MY Character Script Running in BP From C++"));
	}
	GetWorld()->GetTimerManager().SetTimer(TADSHandler, this, &AMyCharacter::ADSCameraFov, 0.01f, true);
	GetWorld()->GetTimerManager().PauseTimer(TADSHandler);
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsCrouched)
	{
		PawnMakeNoise(1.0f, GetActorLocation(), true, this);
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::JumpStart);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyCharacter::JumpEnds);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyCharacter::Sprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyCharacter::SprintingStops);
	PlayerInputComponent->BindAction("ADS", IE_Pressed, this, &AMyCharacter::StartADS);
	PlayerInputComponent->BindAction("ADS", IE_Released, this, &AMyCharacter::StopADS);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyCharacter::OnFire);
	//PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMyCharacter::OnFireStop);
	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &AMyCharacter::OnThrown);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMyCharacter::OnReload);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMyCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMyCharacter::StopCrouch);
}

void AMyCharacter::MoveForward(float value)
{
		if (value != 0.0f)
		{

			if (bIsADS)
			{
				value *= 0.2f;
			}
			else if (!bIsSprinting)
				value *= 0.5f;
			//FVector MovementDirection = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
			//AddMovementInput(GetActorForwardVector(), value);
		
			AddMovementInput(GetActorForwardVector()* value);
		}
}

void AMyCharacter::MoveRight(float value)
{
	if (value != 0.0f)
	{
		if (bIsADS)
		{
			value *= 0.2f;
		}
		/*else if (bIsSprinting)*/
			value *= 0.5f;
		//FVector MovementDirection = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		//AddMovementInput(GetActorRightVector(), value);
		/*const FQuat Rotation = GetActorQuat();
		const FVector Direction = FQuatRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);*/
		AddMovementInput(GetActorRightVector()* value );
	}
}

void AMyCharacter::BeginCrouch()
{
	if (!GetCharacterMovement()->IsCrouching())
	{
		UE_LOG(LogTemp, Warning, TEXT("Crouhing"));
		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Yellow, TEXT("Crouhing"));
		GetCharacterMovement()->bWantsToCrouch = true;
		GetCharacterMovement()->Crouch();
	}
}

void AMyCharacter::StopCrouch()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UE_LOG(LogTemp, Warning, TEXT("UnCrouhing"));
		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Yellow, TEXT("UnCrouhing"));
		GetCharacterMovement()->bWantsToCrouch = false;
		GetCharacterMovement()->UnCrouch();
	}
}

void AMyCharacter::JumpStart()
{
	bIsADS;
	if (!bIsSprinting) 
	{
		bPressedJump = true;
	}
}

void AMyCharacter::JumpEnds()
{
	bIsADS;
	if (bIsSprinting)
	{
		bPressedJump = false;
	}
}

bool AMyCharacter::IsPlayerSprinting()
{
	return bIsSprinting;
}

void AMyCharacter::Sprinting()
{
	bIsSprinting = true;
}

void AMyCharacter::SprintingStops()
{
	bIsSprinting = false;
}

void AMyCharacter::OnThrown()
{
	if (ThrowableClass)
	{
	
			FVector CameraLocation;
			FRotator CameraRotation;
			GetActorEyesViewPoint(CameraLocation, CameraRotation);

			FVector ThrowLocation = CameraLocation + FTransform(CameraRotation).TransformVector(ThrowOffset);
			FRotator ThrowRotation = CameraRotation;

			ThrowRotation.Pitch += 10.f;
			ThrowRotation.Yaw -= 10.f;
			UWorld* World = GetWorld();

			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = this;
				throwabless = GetWorld()->SpawnActor<AThrowObjects>(ThrowableClass, ThrowLocation, ThrowRotation, SpawnParams);
				if (throwabless)
				{
					if (throwabless->ThrowTime())
					{
						FVector ThrownDirection = ThrowRotation.Vector();
						throwabless->ThrowInDirection(ThrownDirection);
					}
				}
			}
			//GetWorldTimerManager().SetTimer(Handle_ThrowWait, this, &AMyCharacter::OnThrown, 3.f);
		
	}
}

void AMyCharacter::OnFire()
{ 
	AActor* MyPlayer = GetOwner();
	if(MyPlayer)
	{
		if (IsPlayerSprinting())
		{
			bIsSprinting=false;
		}
		if (Weapon)
		{
			if (Weapon->Fire())
			{
				UE_LOG(LogTemp, Warning, TEXT("Shooting Weapon"))
					FVector Start = CameraComp->GetComponentLocation();
				FVector Rotation = CameraComp->GetComponentRotation().Vector();
				FVector End = Start + Rotation * 20000.f;

				TArray<FHitResult> HitResults;
				FCollisionResponseParams CollResponse;
				FCollisionQueryParams CollParams;
				CollParams.AddIgnoredActor(this);

				if (GetWorld()->LineTraceMultiByChannel(OUT HitResults, Start, End, COLLISION_TWEAPON, CollParams, CollResponse))
				{
					for (FHitResult& Result : HitResults)
					{
						if (AActor* HitActor = Result.GetActor())
						{
							UGameplayStatics::ApplyPointDamage(HitActor, 40.f, Rotation, Result, MyPlayer->GetInstigatorController(), this, DamageTypes);
							UE_LOG(LogTemp, Warning, TEXT("ActorName:%s"), *HitActor->GetName());
						}
					}
					//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 3.f);
				}
				
				if (FireAnimation != NULL)
				{
					UAnimInstance* AnimInstace = MeshComp->GetAnimInstance();
					if (AnimInstace != NULL)
					{
						
						AnimInstace->Montage_Play(FireAnimation, 1.0f);
						if (bIsADS)
							AnimInstace->Montage_JumpToSection(FName("ArmFireADS"), FireAnimation);
						else
							AnimInstace->Montage_JumpToSection(FName("ArmHipFire"), FireAnimation);
					}
				}
			}
			if (Weapon->PistolAmmo <= 0) {
				UGameplayStatics::PlaySound2D(this, PlayEmptyMag, 3.0f);
				GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::Orange, TEXT("Press 'R' to Reload"));
			}
		}
	}
	
}

//void AMyCharacter::OnFireStop()
//{
//	bCanFire = false;
//}

void AMyCharacter::StartADS()
{
	bIsADS = true;
	GetWorld()->GetTimerManager().UnPauseTimer(TADSHandler);
}

void AMyCharacter::StopADS()
{
	bIsADS = false;
	GetWorld()->GetTimerManager().UnPauseTimer(TADSHandler);
}

bool AMyCharacter::IsPlayerADS()
{
	return bIsADS;
}

void AMyCharacter::ADSCameraFov()
{
	float CameraFov = CameraComp->FieldOfView;
	float NewADSFov = 90.0f;
	if (bIsADS)
		NewADSFov = 75.0f;
	NewADSFov = UKismetMathLibrary::FInterpTo_Constant(CameraFov, NewADSFov, GetWorld()->DeltaTimeSeconds, 100.0f);
	CameraComp->SetFieldOfView(NewADSFov);
}

void AMyCharacter::OnReload()
{
	if (Weapon)
	{
		if (Weapon->PistolAmmo <=0 /*&& Weapon->PistolAmmo<7 *//*&&Weapon->PistolAmmo!=0*/ && Weapon->TotalAmmo!=0)
		{
			if (Weapon->Reload())
			{
				if (ReloadAnimation)
				{
					UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
					if (AnimInstance != NULL)
					{
						AnimInstance->Montage_Play(ReloadAnimation, 1.0f);
						if (Weapon->IsSlideLocked())
						{
							PlayAnimMontage(ReloadAnimation, 1.0f);
						}
					}
				}
			}
		}
	}
}

