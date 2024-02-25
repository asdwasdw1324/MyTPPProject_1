// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, UHealthComponent*, OwningComp, float, NewHealth, float, Delta);
DECLARE_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamageTaking, float);

//Heal parameter struct
USTRUCT(BlueprintType)
struct FHealStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	bool FAutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
	float FHealUpdateTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
	float FHealDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
	float FHealModifier = 2.0f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTPPPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	FHealStruct HealthStruct;

	//For any other damage type to change health
	UFUNCTION(BlueprintCallable, Category = "Damage")
	bool ApplyHealthChange(float Delta);

	//End the game if player died
	UFUNCTION(BlueprintCallable, Category = "Damage")
	bool ApplyGameEnd();

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FOnHealthChanged OnHealthChanged;

	FOnDeath OnDeath;
	FOnDamageTaking OnDamageTaking;

	//Get current health value
	float GetHealth() const {return Health;}

	//Set new health value
	void SetHealth(float NewHealth);

	UFUNCTION(Blueprintcallable, Category = "Damage")
	void OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	//Heal function TimerHandle
	FTimerHandle HealTimerHandle;

	FTimerHandle ProjectileDamageHealTimerHandle;

	void HealUpdate();


		
};
