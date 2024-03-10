// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Animation\AnimMontage.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(HealthCompLog, All, All)

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Health = MaxHealth;

	SetIsReplicatedByDefault(true);

}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamageHandle);
	}

	
}

// Apply damage by base projectile
bool UHealthComponent::ApplyHealthChange(float Delta)
{
	if (Delta <= 0 || ApplyGameEnd() || !GetWorld())
	{
		return false;
	}

	SetHealth(Health - Delta);

	if (ApplyGameEnd())
	{
		OnDeath.Broadcast();
	}
	else if (HealthStruct.FAutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(ProjectileDamageHealTimerHandle, this, &UHealthComponent::HealUpdate, HealthStruct.FHealUpdateTime, true, HealthStruct.FHealDelay);
	}

	return true;
}

// Judge if the character health decrease to zero
bool UHealthComponent::ApplyGameEnd()
{
	if (FMath::IsNearlyZero(Health))
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("You have died, Game Over!!!")));
		UE_LOG(HealthCompLog, Error, TEXT("You have died, Game Over!!!"));

		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(ProjectileDamageHealTimerHandle);
		return true;

	}
	else
	{
		return false;
	}
}

// Apply damage by radius area
void UHealthComponent::OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0 || ApplyGameEnd() || !GetWorld())
	{
		return;
	}
	SetHealth(Health - Damage);

	if (ApplyGameEnd())
	{
		//AddDynamic on the character execute OnDeath function
		OnDeath.Broadcast();
	}
	else if (HealthStruct.FAutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UHealthComponent::HealUpdate, HealthStruct.FHealUpdateTime, true, HealthStruct.FHealDelay);
	}

}

void UHealthComponent::HealUpdate()
{
	SetHealth(Health + HealthStruct.FHealModifier);

	if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(ProjectileDamageHealTimerHandle);
	}
}

void UHealthComponent::SetHealth(float NewHealth)
{
	float LastHealth = GetHealth();
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	float DeltaHealth = LastHealth - Health;

	OnDamageTaking.Broadcast(Health);
	OnHealthChanged.Broadcast(nullptr, this, Health, DeltaHealth);
}