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

					//TimerDelegate
					FTimerDelegate TimerDelegate;
					TimerDelegate.BindLambda([this, Handle = Spec.Handle, ASC = ActorInfo->AbilitySystemComponent]()
					{
						if (ASC.IsValid())
						{
							//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, TEXT("Timer triggered ability cancellation"));
							ASC->CancelAbilityHandle(Handle);
						}
					});
					
					ActorInfo->OwnerActor->GetWorld()->GetTimerManager().SetTimer(RestoreEnhancedAttackTimerHandle, TimerDelegate, 5.0f, false);
					//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Timer set successfully"));
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
	// 添加调用栈信息
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("EndAbility called from: %s"), *FString(__FUNCTION__)));
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("EnhancedAttackActivated?: %s"), bEnhancedAttackActivated ? TEXT("True") : TEXT("False")));
	
	// static int32 CallCount = 0;
	// CallCount++;
	// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("EndAbility Call Count: %d"), CallCount));

	// 检查能力系统组件是否有效
	if (!ActorInfo || !ActorInfo->AbilitySystemComponent.IsValid())
	{
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

	// 清理计时器
	if (RestoreEnhancedAttackTimerHandle.IsValid() && ActorInfo->OwnerActor.IsValid())
	{
		ActorInfo->OwnerActor->GetWorld()->GetTimerManager().ClearTimer(RestoreEnhancedAttackTimerHandle);
	}

	// 直接使用能力系统组件来结束能力
	ASC->CancelAbility(this);
	
	// 清理能力
	ASC->ClearAbility(Handle);
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, FString::Printf(TEXT("Clear Ability Completed for Handle: %s"), *Handle.ToString()));
}




