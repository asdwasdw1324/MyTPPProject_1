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
			if (!bEnhancedAttackActivated)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ability activation failed!"));
				return;
			}
			else
			{
				RestoreEnhancedAttackTimerHandle = FTimerHandle();
				ActorInfo->OwnerActor->GetWorld()->GetTimerManager().SetTimer(RestoreEnhancedAttackTimerHandle, [this, Handle = Spec.Handle, ActorInfo]()
				{
					EndAbility(Handle, ActorInfo, FGameplayAbilityActivationInfo(), true, false);
				}, 5.0f, false);
			}
		}
	}
}

void UWuKongGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("EnhancedAttackActivated?: %s"), bEnhancedAttackActivated ? TEXT("True") : TEXT("False")));
	if (bEnhancedAttackActivated)
	{
		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		//bEnhancedAttackActivated = false;
		if (GetWorld()->GetTimerManager().IsTimerActive(RestoreEnhancedAttackTimerHandle) && ActorInfo->OwnerActor.IsValid())
		{
			ActorInfo->OwnerActor->GetWorld()->GetTimerManager().ClearTimer(RestoreEnhancedAttackTimerHandle);
		}
		
		ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		bEnhancedAttackActivated = false;
		// 添加日志以确认EndAbility被调用的次数
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, FString::Printf(TEXT("EndAbility called. Handle: %s"), *Handle.ToString()));
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, FString::Printf(TEXT("Clear Ability Completed!")));
	}
	else
	{
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Can not Clear Ability!")));
		}
	}
}




