// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnExtensionComponentBase.h"
#include "GameplayTagContainer.h"
#include "PawnCombatComponent.generated.h"

class AEnhancedEffectActor;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class MYTPPPROJECT_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "WuKong|PawnCombat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AEnhancedEffectActor* InWeaponToRegister, bool bRegisterAsEquippedWeapon = false);

	UFUNCTION(BlueprintCallable, Category = "WuKong|PawnCombat")
	void UnRegisterSpawnedWeapon(FGameplayTag InWeaponTagToUnRegister);
	
	UFUNCTION(BlueprintCallable, Category = "WuKong|PawnCombat")
	AEnhancedEffectActor* GetWuKongCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	UPROPERTY(BlueprintReadWrite, Category = "WuKong|PawnCombat")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable, Category = "WuKong|PawnCombat")
	AEnhancedEffectActor* GetWuKongCurrentEquippedWeapon() const;

private:
	TMap<FGameplayTag, AEnhancedEffectActor*> WuKongCarriedWeaponsMap;
};
