// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PowerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnPowerChanged, AActor*, InstigatorActor, UPowerComponent*, OwningComp, float, NewPower, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTPPPROJECT_API UPowerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPowerComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = "Power")
	float Power;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Power")
	float MaxPower = 100.0f;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FOnPowerChanged OnPowerChanged;

	float GetPower() const { return Power; };

	UFUNCTION(BlueprintCallable)
	float SetPower(float NewPower);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = PowerHeal);
	FTimerHandle PowerHealTimerHandle;
	
	UFUNCTION(BlueprintCallable)
	void PowerHealUpdate();
};
