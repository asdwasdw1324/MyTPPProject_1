// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/WuKongHeroGameplayAbility.h"

#include "MyTPPProject/MyTPPProjectCharacter.h"

AMyTPPProjectCharacter* UWuKongHeroGameplayAbility::GetWuKongHeroCharacterFromActorInfo()
{
	if (!CachedWuKongHeroCharacter.IsValid())
	{
		CachedWuKongHeroCharacter = Cast<AMyTPPProjectCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedWuKongHeroCharacter.IsValid()? CachedWuKongHeroCharacter.Get() : nullptr;
}

AController* UWuKongHeroGameplayAbility::GetWuKongHeroControllerFromActorInfo()
{
	if (!CachedWuKongHeroController.IsValid())
	{
		CachedWuKongHeroController = Cast<AController>(CurrentActorInfo->PlayerController);
	}
	return CachedWuKongHeroController.IsValid()? CachedWuKongHeroController.Get() : nullptr;
}

UWuKongCombatComponent* UWuKongHeroGameplayAbility::GetWuKongCombatComponentFromActorInfo()
{
	return GetWuKongHeroCharacterFromActorInfo()->WuKongCombatComponent;
}
