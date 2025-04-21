// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PawnCombatComponent.h"
#include "EnhancedEffectActor.h"

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AEnhancedEffectActor* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	check(InWeaponToRegister);

	// Check if the weapon tag already exists in the TMap
	if (WuKongCarriedWeaponsMap.Contains(InWeaponTagToRegister))
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Tag %s already exists!"), *InWeaponTagToRegister.ToString());
		return;
	}

	// Register the weapon to this TMap
	WuKongCarriedWeaponsMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

	const FString WeaponString = FString::Printf(TEXT("A weapon named: %s has registered by the tag: %s"), *InWeaponToRegister->GetName(), *InWeaponTagToRegister.ToString());
	UE_LOG(LogTemp, Warning, TEXT("%s"), *WeaponString);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, WeaponString);
	
	// If bRegisterAsEquippedWeapon is true, set the CurrentEquippedWeaponTag to the new weapon tag
	if (bRegisterAsEquippedWeapon)
	{
		if (InWeaponTagToRegister.IsValid())
		{
			CurrentEquippedWeaponTag = InWeaponTagToRegister;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("CurrentEquippedWeaponTag set to: %s"), *CurrentEquippedWeaponTag.ToString()));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("InWeaponTagToRegister is invalid!"));
		}
	}
	
}

void UPawnCombatComponent::UnRegisterSpawnedWeapon(FGameplayTag InWeaponTagToUnRegister)
{
	if (InWeaponTagToUnRegister.IsValid())
	{
		if (WuKongCarriedWeaponsMap.Contains(InWeaponTagToUnRegister))
		{
			if (WuKongCarriedWeaponsMap.Remove(InWeaponTagToUnRegister) > 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("A weapon has been unregistered by the tag: %s"), *InWeaponTagToUnRegister.ToString()));
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("InWeaponTagToUnRegister is not unregistered: %s!"), *InWeaponTagToUnRegister.ToString());
			UE_LOG(LogTemp, Warning, TEXT("Weapon with tag %s does not exist!"), *InWeaponTagToUnRegister.ToString());
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("InWeaponTagToUnRegister is invalid!"));
	}
}

AEnhancedEffectActor* UPawnCombatComponent::GetWuKongCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	if (WuKongCarriedWeaponsMap.Contains(InWeaponTagToGet))
	{
		if (auto FoundWeapon = WuKongCarriedWeaponsMap.Find(InWeaponTagToGet))
		{
			return *FoundWeapon;
		}
	}
	return nullptr;
}

AEnhancedEffectActor* UPawnCombatComponent::GetWuKongCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}

	return GetWuKongCarriedWeaponByTag(CurrentEquippedWeaponTag);
}
