// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/WuKongGameplayAbility.h"
#include "AbilitySystem/WuKongAbilitySystemComponent.h"
#include "DSP/Delay.h"

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
	
	// 调用EndAbility函数
	if (bEnhancedAttackActivated)
	{
		EndAbility(Spec.Handle, ActorInfo, FGameplayAbilityActivationInfo(), false, false);
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
			bEnhancedAttackActivated = false;
			// 添加日志以确认EndAbility被调用的次数
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("EndAbility called. Handle: %s"), *Handle.ToString()));
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, FString::Printf(TEXT("Clear Ability Completed!")));
		}, 5.0f, false);  // 5秒后恢复
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, FString::Printf(TEXT("Can not Clear Ability!")));
	}
	
}




