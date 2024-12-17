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
			bEnhancedAttackActivated = ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UWuKongGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (ActorInfo)
	{
		FTimerHandle RestoreEnhancedAttackTimerHandle;
		ActorInfo->OwnerActor->GetWorld()->GetTimerManager().SetTimer(RestoreEnhancedAttackTimerHandle, [this, Handle, AbilitySystemComponent = ActorInfo->AbilitySystemComponent]()
		{
			AbilitySystemComponent->ClearAbility(Handle);
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, FString::Printf(TEXT("Clear Ability Completed!")));
		}, 5.0f, false);  // 5秒后恢复
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, FString::Printf(TEXT("Can not Clear Ability!")));
	}
}




