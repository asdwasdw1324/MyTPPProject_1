// Fill out your copyright notice in the Description page of Project Settings.


#include "GeometryHubActor.h"
#include "Engine/World.h"

// Sets default values
AGeometryHubActor::AGeometryHubActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGeometryHubActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetWorld())
	{
		SpawnActorMethod1();
		SpawnActorMethod2();
	}
}

// Called every frame
void AGeometryHubActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGeometryHubActor::SpawnActorMethod1()
{
	for (int32 i = 0; i < 5; ++i)
	{
		//Spawn Location FTransform
		FTransform GemometryTransform = FTransform(FRotator::ZeroRotator, FVector(0.0f, 300.0f * i, 300.0f));

		//Return Pointer that spawned by SpawnActor function
		ABaseGeometryActor* Geometry = GetWorld()->SpawnActorDeferred<ABaseGeometryActor>(GeometryClass, GemometryTransform);

		if (Geometry)
		{
			FGeometryData Data;
			Data.GeoMoveType = FMath::RandBool() ? EMovementType::Static : EMovementType::Sin;
			Data.GeoColor = FLinearColor::MakeRandomColor();
			Geometry->SetGeometryData(Data);
			Geometry->FinishSpawning(GemometryTransform);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Geometry Pointer!"));
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, (TEXT("No Geometry Pointer!")));
		}
	}
}

void AGeometryHubActor::SpawnActorMethod2()
{
	for (const FGeometryPayload Payload : GeometryPayloads)
	{
		ABaseGeometryActor* Geometry = GetWorld()->SpawnActorDeferred<ABaseGeometryActor>(Payload.GeometryClass, Payload.InitialTransform);

		if (Geometry)
		{
			Geometry->SetGeometryData(Payload.Data);
			Geometry->FinishSpawning(Payload.InitialTransform);
		}
	}
}

