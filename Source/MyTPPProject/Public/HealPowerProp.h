// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WuKongInterface_Prop.h"
#include "HealPowerProp.generated.h"

class USceneComponent;

UCLASS()
class MYTPPPROJECT_API AHealPowerProp : public AActor, public IWuKongInterface_Prop
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealPowerProp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parameter)
	float TargetPitch;

	virtual void PropInteract_Implementation(APawn* InstigatorPawn);

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle DestroyTimerHandle;
	void DestroySelf();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HealPowerActor)
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HealPowerActor)
	UStaticMeshComponent* LidMesh;

};
