// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/WuKongGameplayAbility.h"
#include "AbilitySystem/WuKongAbilitySystemComponent.h"

void UWuKongGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivationPolicy == EWuKongAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UWuKongGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AbilityActivationPolicy == EWuKongAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

void UWuKongGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AbilityActivationPolicy == EWuKongAbilityActivationPolicy::OnTriggered)
	{
		FGameplayAbilitySpec* Spec = ActorInfo->AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
		if (ActorInfo && !(*Spec).IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Handle);
		}

		FTimerHandle RestoreEnhancedAttackTimerHandle;
		ActorInfo->OwnerActor->GetWorld()->GetTimerManager().SetTimer(RestoreEnhancedAttackTimerHandle, [this, ActorInfo, Handle]()
		{
			if (AbilityActivationPolicy == EWuKongAbilityActivationPolicy::OnTriggered)
			{
				if (ActorInfo)
				{
					ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
				}
			}
		}, 10.0f, false);  // 10秒后恢复
	}
}



