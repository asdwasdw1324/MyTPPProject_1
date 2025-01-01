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
	UFUNCTION(BlueprintCallable, Category = "Pawn Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AEnhancedEffectActor* InWeaponToRegister, bool bInShouldBeEquipped = true);

	UFUNCTION(BlueprintCallable, Category = "Pawn Combat")
	void UnRegisterSpawnedWeapon(FGameplayTag InWeaponTagToUnRegister);
	
	UFUNCTION(BlueprintCallable, Category = "Pawn Combat")
	AEnhancedEffectActor* GetWuKongCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	UPROPERTY(BlueprintReadWrite, Category = "Pawn Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable, Category = "Pawn Combat")
	AEnhancedEffectActor* GetWuKongCurrentEquippedWeapon() const;

private:
	TMap<FGameplayTag, AEnhancedEffectActor*> WuKongCarriedWeaponsMap;
};
