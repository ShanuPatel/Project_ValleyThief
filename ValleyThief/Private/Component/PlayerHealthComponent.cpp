// Fill out your copyright notice in the Description page of Project Settings.


#include "ValleyThief/Public/Component/PlayerHealthComponent.h"
#include "Components/ActorComponent.h"

// Sets default values for this component's properties
UPlayerHealthComponent::UPlayerHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	DefaultHealth = 100.f;
	// ...
}


// Called when the game starts
void UPlayerHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* MyPlayer = GetOwner();
	if (MyPlayer)
	{
		MyPlayer->OnTakeAnyDamage.AddDynamic(this, &UPlayerHealthComponent::HandleTAkeAnyDamage);
	}
	Health=DefaultHealth;
	
}

void UPlayerHealthComponent::HandleTAkeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage < 0.0f)
	{
		return;
	}
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	UE_LOG(LogTemp, Log, TEXT("HealthComponent Remaining: %s"), *FString::SanitizeFloat(Health));
	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}


// Called every frame
//void UPlayerHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

