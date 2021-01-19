// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ValleyThiefGameModeBase.h"
#include "ValleyThief/ValleyThief.h"
#include "ValleyThief/Public/BaseHUD.h"
#include "ValleyThief/Private/MyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"
void AValleyThiefGameModeBase::StartPlay()
{
	{
		Super::StartPlay();
	
		if (GEngine)
		{
			UGameplayStatics::PlaySound2D(this, PlayWind,3.0);
			//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Purple, TEXT("Test message Sample"));
		}
	}
}

AValleyThiefGameModeBase::AValleyThiefGameModeBase()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Character/Blueprints/BP_MyCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ABaseHUD::StaticClass();
}

void AValleyThiefGameModeBase::CompleteMission(APawn* InstigatorPawn, bool bIsMissionComplete)
{
	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);
		if (SpectatingCameraClass)
		{
			TArray<AActor*> ListAllActor;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingCameraClass, ListAllActor);
			if (ListAllActor.Num() > 0)
			{
				AActor* NewCameraView = ListAllActor[0];
				APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());
				if (PC)
				{
					PC->SetViewTargetWithBlend(NewCameraView, 1.5f, EViewTargetBlendFunction::VTBlend_Cubic);
					PC->bShowMouseCursor =true;
					PC->bEnableClickEvents = true;
					PC->bEnableMouseOverEvents = true;
				}
			}

		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Not Spectating because is nullptr, and the pawn player is not valid"))
		}
	}
	OnMissionComplete(InstigatorPawn, bIsMissionComplete);
}
