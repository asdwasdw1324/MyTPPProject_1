// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/WuKongAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WuKongGameplayAbility.h"


void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UWuKongAbilitySystemComponent* ASCToGive, int32 ApplyLevel)
{
	ensureMsgf(ASCToGive, TEXT("AbilitySystemComponent is empty!"));

	GrantAbilities(ActivateOnGivenAbilities, ASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, ASCToGive, ApplyLevel);
	
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UWuKongGameplayAbility>>& AbilitiesToGive, UWuKongAbilitySystemComponent* ASCToGive, int32 ApplyLevel)
{
	if (AbilitiesToGive.IsEmpty() || !ASCToGive)
	{
		return;
	}

	for (const TSubclassOf<UWuKongGameplayAbility>& Ability : AbilitiesToGive)
	{
		if(!Ability) continue;

		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = ASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		
		ASCToGive->GiveAbility(AbilitySpec);
	}
}
