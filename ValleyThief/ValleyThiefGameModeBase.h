// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ValleyThiefGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class VALLEYTHIEF_API AValleyThiefGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
		virtual void StartPlay() override;
	/*Changing From here*/
protected:
	UPROPERTY(EditDefaultsOnly, Category="Spectating Camera")
	TSubclassOf<AActor> SpectatingCameraClass;
	UPROPERTY(EditDefaultsOnly, Category = " SoundFX")
		USoundBase* PlayWind;

public:
	AValleyThiefGameModeBase();
	void CompleteMission(APawn* InstigatorPawn, bool bIsMissionComplete);

	UFUNCTION(BlueprintImplementableEvent, Category ="Gameplay")
	void OnMissionComplete(APawn* InstigatorPawn, bool bIsMissionComplete);
};
