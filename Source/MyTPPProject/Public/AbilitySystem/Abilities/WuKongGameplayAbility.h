// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "WuKongGameplayAbility.generated.h"

class UPawnCombatComponent;
class UWuKongAbilitySystemComponent;


UENUM(BlueprintType)
enum class EWuKongAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};

/**
 * 
 */
UCLASS()
class MYTPPPROJECT_API UWuKongGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	bool bEnhancedAttackActivated;
	
	FTimerHandle RestoreEnhancedAttackTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "WuKongAbility")
	EWuKongAbilityActivationPolicy AbilityActivationPolicy = EWuKongAbilityActivationPolicy::OnGiven;

	UFUNCTION(BlueprintPure, Category = "WuKong|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "WuKong|Ability")
	UWuKongAbilitySystemComponent* GetWuKongAbilitySystemComponentFromActorInfo() const;
	
	
};
