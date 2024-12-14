// Fill out your copyright notice in the Description page of Project Settings.


#include "EnhancedEffectActor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnhancedEffectActor::AEnhancedEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponMesh"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	EffectComp->SetupAttachment(CollisionBox);
	
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Component"));
	AudioComp->SetupAttachment(CollisionBox);
}



