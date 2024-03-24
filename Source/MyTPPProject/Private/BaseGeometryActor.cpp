// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGeometryActor.h"
#include "UObject\ConstructorHelpers.h"
#include "TimerManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components\StaticMeshComponent.h"
#include "Engine\StaticMesh.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseGeometry, All, All)

// Sets default values
ABaseGeometryActor::ABaseGeometryActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>BaseStaticMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
    if (ensure(BaseStaticMesh.Succeeded()))
    {
	    BaseMesh->SetStaticMesh(BaseStaticMesh.Object);
    }

	// static ConstructorHelpers::FObjectFinder<UMaterial>BaseMaterial(TEXT("/Script/Engine.Material'/Game/StarterContent/Materials/M_Basic_Wall.M_Basic_Wall'"));
 //    if (BaseMaterial.Succeeded())
 //    {
	//     UMaterialInstanceDynamic* BaseMaterialInstanceDynamic = UMaterialInstanceDynamic::Create(BaseMaterial.Object, BaseMesh);
 //    	BaseMesh->SetMaterial(0, BaseMaterialInstanceDynamic);
 //    }
	
	MyEnum = EMyEnumeration::Type1;
	
}

// Called when the game starts or when spawned
void ABaseGeometryActor::BeginPlay()
{
	Super::BeginPlay();

	GeoInitialLocation = GetActorLocation();
	
	SetColor(GeometryData.GeoColor);
	GetWorldTimerManager().SetTimer(ColorChangeTimerHandle, this, &ABaseGeometryActor::OnTimeFire, GeometryData.ChangeColorTimeRate, true, -1);
}

// Called every frame
void ABaseGeometryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveLogic();
}

void ABaseGeometryActor::MoveLogic()
{
	switch (GeometryData.GeoMoveType)
	{
	case EMovementType::Sin:
		{
			FVector CurrentLocation = GetActorLocation();
			if (GetWorld())
			{
				float Time = GetWorld()->GetTimeSeconds();
				CurrentLocation.Z = GeoInitialLocation.Z + GeometryData.GeoAmplitude * FMath::Sin(GeometryData.GeoFrequency * Time);

				SetActorLocation(CurrentLocation);
			}
		}
		break;

	case EMovementType::Static: break;
	default: break;
	}
}

void ABaseGeometryActor::SetColor(const FLinearColor& Color)
{
	if (!BaseMesh) return;

	UMaterialInstanceDynamic* DynMaterial = BaseMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (DynMaterial)
	{
		DynMaterial->SetVectorParameterValue("Color", Color);
	}
}

void ABaseGeometryActor::OnTimeFire()
{
	if (++TimerCount <= MaxTimerCount)
	{
		const FLinearColor NewColor = FLinearColor::MakeRandomColor();
		
		SetColor(NewColor);
		OnColorChanged.Broadcast(NewColor, GetName());
	}
	else
	{
		GetWorldTimerManager().ClearTimer(ColorChangeTimerHandle);
		OnTimerFinished.Broadcast(this);
	}
}

void ABaseGeometryActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnColorChanged.AddDynamic(this, &ABaseGeometryActor::ColorChangeFunc);
	OnTimerFinished.AddUObject(this, &ABaseGeometryActor::TimerFinishedFunc);
}

void ABaseGeometryActor::ColorChangeFunc(const FLinearColor& Color, const FString& Name)
{
	UE_LOG(LogBaseGeometry, Warning, TEXT("Color set up to: %s"), *Color.ToString());
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, (TEXT("Color set up to: %s"), *Color.ToString()));
}

void ABaseGeometryActor::TimerFinishedFunc(AActor* FinishedGeoActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, (TEXT("%s: Stop to change color!"), *GetNameSafe(FinishedGeoActor)));
	UE_LOG(LogBaseGeometry, Error, TEXT("%s: Stop to change color!"), *GetNameSafe(FinishedGeoActor));
}
