// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGeometryActor.h"
#include "GeometryHubActor.generated.h"

USTRUCT(BlueprintType)
struct FGeometryPayload
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
	TSubclassOf<ABaseGeometryActor> GeometryClass;

	UPROPERTY()
	FGeometryData Data;

	UPROPERTY()
	FTransform InitialTransform;
};

UCLASS()
class MYTPPPROJECT_API AGeometryHubActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGeometryHubActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//The container for the 1 SpawnActor method
	UPROPERTY(EditAnywhere, Category = "SpawnActor1")
	TSubclassOf<ABaseGeometryActor> GeometryClass;

	//The container for the 2 SpawnActor method
	UPROPERTY(EditAnywhere, Category = "SpawnActor2")
	TArray<FGeometryPayload> GeometryPayloads;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SpawnActorMethod1();
	
	UFUNCTION(BlueprintCallable)
	void SpawnActorMethod2();
};
