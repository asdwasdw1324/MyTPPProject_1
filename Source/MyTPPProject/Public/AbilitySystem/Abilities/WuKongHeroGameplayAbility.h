// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WuKongGameplayAbility.h"
#include "WuKongHeroGameplayAbility.generated.h"

class AMyTPPProjectCharacter;
class AController;

/**
 * 
 */
UCLASS()
class MYTPPPROJECT_API UWuKongHeroGameplayAbility : public UWuKongGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "WuKong|Ability")
	AMyTPPProjectCharacter* GetWuKongHeroCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "WuKong|Ability")
	AController* GetWuKongHeroControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "WuKong|Ability")
	UWuKongCombatComponent* GetWuKongCombatComponentFromActorInfo();

private:
	TWeakObjectPtr<AMyTPPProjectCharacter> CachedWuKongHeroCharacter;
	TWeakObjectPtr<AController> CachedWuKongHeroController;
	
};
