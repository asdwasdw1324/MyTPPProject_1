// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WuKongAbilitySystemComponent.h"

void UWuKongAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// ReSharper disable once CppDeprecatedEntity
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag)) continue;

		TryActivateAbility(AbilitySpec.Handle);
		
	}	
}

void UWuKongAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	
}
