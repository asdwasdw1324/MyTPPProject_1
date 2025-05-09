// Fill out your copyright notice in the Description page of Project Settings.

#include "DataAsset/DataAsset_WuKongStartUpDataBase.h"
#include "AbilitySystem/WuKongAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WuKongGameplayAbility.h"

bool FWuKongHeroAbilitySet::IsValid() const
{
    return InputTag.IsValid() && AbilityToGrant;
}

void UDataAsset_WuKongStartUpDataBase::GiveToAbilitySystemComponent(UWuKongAbilitySystemComponent* ASCToGive, int32 ApplyLevel)
{
    Super::GiveToAbilitySystemComponent(ASCToGive, ApplyLevel);

    for (const FWuKongHeroAbilitySet& AbilitySet : WuKongStartUpAbilitySets)
    {
        if (!AbilitySet.IsValid()) continue;

        FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
        AbilitySpec.SourceObject = ASCToGive->GetAvatarActor();
        AbilitySpec.Level = ApplyLevel;
        AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);
        
        ASCToGive->GiveAbility(AbilitySpec);
    }
}
