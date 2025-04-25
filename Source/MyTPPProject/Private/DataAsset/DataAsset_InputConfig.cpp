// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/DataAsset_InputConfig.h"

UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	for (const FWuKongInputActionConfig& NativeInputAction : NativeInputActions)
	{
		if (NativeInputAction.InputTag == InInputTag && NativeInputAction.InputAction)
		{
			return NativeInputAction.InputAction;
		}
	}

	return nullptr;
}

UInputAction* UDataAsset_InputConfig::FindAbilityInputActionByTag(const FGameplayTag& InInputTag) const
{
	for (const FWuKongInputActionConfig& AbilityInputAction : AbilityInputActions)
	{
		if (AbilityInputAction.InputTag == InInputTag && AbilityInputAction.InputAction)
		{
			return AbilityInputAction.InputAction;
		}
	}
	return nullptr;
}