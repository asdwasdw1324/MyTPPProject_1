// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/WuKongGameplayAbility.h"
#include "AbilitySystem/WuKongAbilitySystemComponent.h"

void UWuKongGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivationPolicy == EWuKongAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid() && !Spec.IsActive())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Attempting to activate ability..."));
			
			// 尝试激活能力
			bEnhancedAttackActivated = ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
			
			if (bEnhancedAttackActivated)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Ability activated successfully"));
				
				// 只有在成功激活后才设置计时器
				if (ActorInfo->OwnerActor.IsValid())
				{
					// 确保之前的计时器被清理
					if (RestoreEnhancedAttackTimerHandle.IsValid())
					{
						ActorInfo->OwnerActor->GetWorld()->GetTimerManager().ClearTimer(RestoreEnhancedAttackTimerHandle);
					}

					FTimerDelegate TimerDelegate;
					TimerDelegate.BindLambda([this, Handle = Spec.Handle, ActorInfo]()
					{
						if (bEnhancedAttackActivated)  // 确保状态仍然是激活的
						{
							EndAbility(Handle, ActorInfo, FGameplayAbilityActivationInfo(), true, false);
						}
					});
					
					ActorInfo->OwnerActor->GetWorld()->GetTimerManager().SetTimer(
						RestoreEnhancedAttackTimerHandle,
						TimerDelegate,
						5.0f,
						false
					);
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Failed to activate Enhanced Attack Ability"));
			}
		}
	}
}

void UWuKongGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// 检查能力系统组件是否有效
	if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("AbilitySystemComponent not valid"));
		return;
	}

	// 获取能力系统组件
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	
	// 检查能力是否仍然激活
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(Handle);
	if (!Spec || !Spec->IsActive())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Ability spec not found or not active"));
		return;
	}

	// 如果我们的增强状态已经关闭，直接返回
	if (!bEnhancedAttackActivated)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Enhanced attack already deactivated"));
		return;
	}

	// 标记我们的增强状态为关闭
	bEnhancedAttackActivated = false;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Enhanced Attack Status set to false"));

	// 清理计时器
	if (RestoreEnhancedAttackTimerHandle.IsValid() && ActorInfo->OwnerActor.IsValid())
	{
		ActorInfo->OwnerActor->GetWorld()->GetTimerManager().ClearTimer(RestoreEnhancedAttackTimerHandle);
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Timer cleared"));
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}




