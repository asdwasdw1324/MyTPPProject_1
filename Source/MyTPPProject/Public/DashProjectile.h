// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "DashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class MYTPPPROJECT_API ADashProjectile : public ABaseProjectile
{
	GENERATED_BODY()

public:

	ADashProjectile();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Param")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	USoundBase* EmitterSound;

	// Handle to cancel timer if we already hit something
	FTimerHandle TimerHandle_DelayedDetonate;

	// Base class using BlueprintNativeEvent, we must override the _Implementation not the Explode()
	virtual void Explode_Implementation() override;

	void TeleportInstigator();

	virtual void BeginPlay() override;
	
};
