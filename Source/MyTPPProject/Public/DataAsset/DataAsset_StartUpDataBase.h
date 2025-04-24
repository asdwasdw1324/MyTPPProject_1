// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UWuKongGameplayAbility;
class UWuKongAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class MYTPPPROJECT_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UWuKongAbilitySystemComponent* ASCToGive,int32 ApplyLevel = 1);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UWuKongGameplayAbility>> ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UWuKongGameplayAbility>> ReactiveAbilities;

	void GrantAbilities(const TArray<TSubclassOf<UWuKongGameplayAbility>>& AbilitiesToGive,UWuKongAbilitySystemComponent* ASCToGive,int32 ApplyLevel = 1);
	
};
