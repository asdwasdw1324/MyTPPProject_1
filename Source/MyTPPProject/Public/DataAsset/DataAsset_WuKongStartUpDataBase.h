// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/DataAsset_StartUpDataBase.h"
#include "GameplayTagContainer.h"
#include "DataAsset_WuKongStartUpDataBase.generated.h"

USTRUCT(BlueprintType)
struct FWuKongHeroAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UWuKongGameplayAbility>> AbilityToGrant;

	bool IsValid() const;
};

/**
 * 
 */
UCLASS()
class MYTPPPROJECT_API UDataAsset_WuKongStartUpDataBase : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UWuKongAbilitySystemComponent* ASCToGive,int32 ApplyLevel = 1);

private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData", meta = (TitleProperty = "InputTag"))
	TArray<FWuKongHeroAbilitySet> WuKongStartUpAbilitySets;
};
