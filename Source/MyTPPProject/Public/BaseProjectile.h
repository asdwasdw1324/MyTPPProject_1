// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UAudioComponent;
class USoundCue;
class UCameraShakeBase;

USTRUCT(BlueprintType)
struct FCameraShakeStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects|Shake")
	TSubclassOf<UCameraShakeBase>ImpactShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects|Shake")
	float ImpactShakeInnerRadius;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects|Shake")
	float ImpactShakeOuterRadius;
};


UCLASS(ABSTRACT)
class MYTPPPROJECT_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UParticleSystem> ImpactVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<USoundCue> ImpactSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> MoveComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UParticleSystemComponent> EffectComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UAudioComponent> AudioComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	FCameraShakeStruct CameraShakeStruct;

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	// BlueprintNativeEvent = C++ base implementation, can be expanded in Blueprints
	// BlueprintCallable to allow child classes to trigger
	// Not required for assignment, useful for expanding in Blueprint later on
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();
	
	virtual void Explode_Implementation();

	virtual void PostInitializeComponents() override;
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
