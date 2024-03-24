// Fill out your copyright notice in the Description page of Project Settings.


#include "FireDamageArea.h"
#include "Components\SceneComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFireDamageArea::AFireDamageArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	FireDamageArea = CreateDefaultSubobject<USceneComponent>(TEXT("FireArea"));
	RootComponent = FireDamageArea;
}

// Called when the game starts or when spawned
void AFireDamageArea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFireDamageArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetActorLocation(), FDAParam.FireRadius, 48, FDAParam.AreaFrameColor);
	UGameplayStatics::ApplyRadialDamage(GetWorld(), FDAParam.DamageValuePS, GetActorLocation(), FDAParam.FireRadius, FDAParam.DamageType, FDAParam.RadialIgnoreActors, this, nullptr, false, ECollisionChannel::ECC_Visibility);
}

