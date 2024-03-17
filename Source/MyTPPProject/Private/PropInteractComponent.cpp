// Fill out your copyright notice in the Description page of Project Settings.


#include "PropInteractComponent.h"
#include "WuKongInterface_Prop.h"

// Sets default values for this component's properties
UPropInteractComponent::UPropInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UPropInteractComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectiveQueryParam;
	ObjectiveQueryParam.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectiveQueryParam.AddObjectTypesToQuery(ECC_WorldStatic);

	AActor* MyOwner = GetOwner();
	check(MyOwner);
	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 1000);

	//Contain hit results in this array
	TArray<FHitResult>Hits;
	
	float CollisionRadius = 30.0f;
	FCollisionShape Shape;
	Shape.SetSphere(CollisionRadius);

	//返回碰撞布尔类型碰撞检测结果
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, TraceEnd, FQuat::Identity, ObjectiveQueryParam, Shape);

	//根据碰撞检测结果确定检测射线的颜色，真为绿，假为红
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;


	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (ensure(HitActor))
		{
			if (HitActor->Implements<UWuKongInterface_Prop>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				IWuKongInterface_Prop::Execute_PropInteract(HitActor, MyPawn);
				IWuKongInterface_Prop::Execute_GetInteractText(HitActor, MyPawn);
				break;
			}
		}
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, CollisionRadius, 32, LineColor, false, 2.0f, 1.0f);
	}
	DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, LineColor, false, 2.0f, 2.0f);
}


// Called when the game starts
void UPropInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPropInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

