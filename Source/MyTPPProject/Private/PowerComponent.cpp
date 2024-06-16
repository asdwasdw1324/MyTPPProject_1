// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerComponent.h"
#include "HealthComponent.h"
#include "MyTPPProject\MyTPPProjectCharacter.h"

// Sets default values for this component's properties
UPowerComponent::UPowerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Power = MaxPower;

	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UPowerComponent::BeginPlay()
{
	Super::BeginPlay();

	SetPower(MaxPower);
	// ...
	
}


// Called every frame
void UPowerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPowerComponent::PowerHealUpdate()
{
	SetPower(Power + 2.0f);

	AMyTPPProjectCharacter* PlayerWuKong = Cast<AMyTPPProjectCharacter>(GetOwner());
	float PlayerWuKongHealth = PlayerWuKong->TppHealthComponent->GetHealth();

	if (UWorld* World = GetWorld())
	{
		if ((FMath::IsNearlyEqual(Power, MaxPower)) || (FMath::IsNearlyZero(PlayerWuKongHealth)))
		{
			World->GetTimerManager().ClearTimer(PowerHealTimerHandle);
		}
	}

}

float UPowerComponent::SetPower(float NewPower)
{
	float LastPower = GetPower();
	Power = FMath::Clamp(NewPower, 0.0f, MaxPower);
	float DeltaPower = Power - LastPower;

	OnPowerChanged.Broadcast(nullptr, this, Power, DeltaPower);

	return Power;
}