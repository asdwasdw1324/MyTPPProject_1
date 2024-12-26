// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(HealthCompLog, All, All)

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	bHasTriggeredDeath = false;

	SetIsReplicatedByDefault(true);

	// if (AActor* ComponentOwnerCharacter = GetOwner())
	// {
	// 	ComponentOwnerCharacter->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamageHandle);
	// }
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHealth(MaxHealth);
}

// Apply damage by base projectile
bool UHealthComponent::ApplyHealthChange(const float Delta)
{
	if (Delta <= 0 || !GetWorld() || bHasTriggeredDeath)
	{
		return false;
	}

	SetHealth(Health - Delta);
	
	if (ApplyGameEnd() && bHasTriggeredDeath)
	{
		if (OnDeath.IsBound())
		{
			OnDeath.Broadcast();
		}
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
		// 确保在世界有效的情况下清理计时器
		if (UWorld* World = GetWorld())
		{
			FTimerManager& TimerManager = World->GetTimerManager();
			
			// 清理所有相关计时器
			if (ProjectileDamageHealTimerHandle.IsValid())
			{
				TimerManager.ClearTimer(ProjectileDamageHealTimerHandle);
			}
		}

		// 设置死亡标记
		bHasTriggeredDeath = true;
		
		return true;
	}
	return false;
}

//Health recover automatically
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
	float ClampedHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);

	if (FMath::IsNearlyEqual(ClampedHealth,Health) && ClampedHealth != MaxHealth)
	{
		return;
	}
	
	float LastHealth = GetHealth();
	Health = ClampedHealth;
	float DeltaHealth = LastHealth - Health;

	//OnDamageTaking.Broadcast(Health);
	OnHealthChanged.Broadcast(GetOwner(), this, Health, DeltaHealth);
	
}

// Apply damage by radius area
// bool UHealthComponent::OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
// {
// 	if (Damage <= 0 || !GetWorld() || bHasTriggeredDeath)
// 	{
// 		return false;
// 	}
//
// 	SetHealth(Health - Damage);
//
// 	if (ApplyGameEnd())
// 	{
// 		if (!bHasTriggeredDeath)
// 		{
// 			OnDeath.Broadcast();
// 			bHasTriggeredDeath = true;
// 		}
// 	}
// 	else if (HealthStruct.FAutoHeal)
// 	{
// 		GetWorld()->GetTimerManager().SetTimer(ProjectileDamageHealTimerHandle, this, &UHealthComponent::HealUpdate, HealthStruct.FHealUpdateTime, true, HealthStruct.FHealDelay);
// 	}
// 	
// 	return true;
// }