// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "WuKongGameplayAbility.generated.h"

class UPawnCombatComponent;


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

	static bool bIsEndingAbility;
	
	bool bEnhancedAttackActivated = false;
	
	FTimerHandle RestoreEnhancedAttackTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category = "WuKongAbility")
	EWuKongAbilityActivationPolicy AbilityActivationPolicy = EWuKongAbilityActivationPolicy::OnTriggered;

	UFUNCTION(BlueprintPure, Category = "WuKongAbility")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;
	
	
};
