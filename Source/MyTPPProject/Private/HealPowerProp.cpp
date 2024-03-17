// Fill out your copyright notice in the Description page of Project Settings.


#include "HealPowerProp.h"
#include "MyTPPProject\MyTPPProjectCharacter.h"
#include "PowerComponent.h"

// Sets default values
AHealPowerProp::AHealPowerProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.0f;
}

void AHealPowerProp::PropInteract_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));

	//UE_LOG(LogTemp, Warning, TEXT("TriggerInteract!"));

	AMyTPPProjectCharacter* WuKong = CastChecked<AMyTPPProjectCharacter>(InstigatorPawn);
	float currentpower = WuKong->TPPPowerComponent->GetPower();
	WuKong->TPPPowerComponent->SetPower(currentpower + 25.0f);
	
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AHealPowerProp::DestroySelf, 2.0f, false);

}

FText AHealPowerProp::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn)
	{
		FText InstigatorPawnName =  FText::FromString(GetNameSafe(InstigatorPawn));
		UE_LOG(LogTemp, Warning, TEXT("GetInteractText: %s"), *GetNameSafe(InstigatorPawn))
		return InstigatorPawnName;
	}
	
	return FText::GetEmpty();
}

// Called when the game starts or when spawned
void AHealPowerProp::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHealPowerProp::DestroySelf()
{
	Destroy();

	GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);
}

// Called every frame
void AHealPowerProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

