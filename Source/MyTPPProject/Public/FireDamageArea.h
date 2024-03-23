// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireDamageArea.generated.h"

class USceneComponent;

UCLASS()
class MYTPPPROJECT_API AFireDamageArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireDamageArea();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> FireDamageArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRadius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor AreaFrameColor = FColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageValuePS = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<AActor*> RadialIgnoreActors;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
