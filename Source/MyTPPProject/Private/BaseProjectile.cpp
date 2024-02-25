// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"
#include "Components\SphereComponent.h"
#include "GameFramework\ProjectileMovementComponent.h"
#include "Particles\ParticleSystemComponent.h"
#include "Components\AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet\GameplayStatics.h"
#include "HealthComponent.h"


// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setup SphereComponent as root component
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;
	//Set Projectile collision type in the UE Editor
	SphereComp->SetCollisionProfileName("Projectile");

	//Setup ParticleSystemComponent attach to SphereComponent
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectComp"));
	EffectComp->SetupAttachment(RootComponent);

	//Setup AudioComponent attach to SphereComponent
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);

	//Setup ProjectileMovementComponent for this BaseProjectile actor, otherwise it cannot move
	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoveComp"));
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->bInitialVelocityInLocalSpace = true;
	MoveComp->ProjectileGravityScale = 1.0f;
	MoveComp->InitialSpeed = 8000;
	MoveComp->bShouldBounce = true;
	MoveComp->Bounciness = 0.8f;

	//Setup CameraShake parameter
	CameraShakeStruct.ImpactShakeInnerRadius = 800.0f;
	CameraShakeStruct.ImpactShakeOuterRadius = 1500.0f;

	// Directly set bool instead of going through SetReplicates(true) within constructor,
	// Only use SetReplicates() outside constructor
	bReplicates = true;

}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Hit function when ProjectileBase hit on any other actors
void ABaseProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();

	if (OtherActor)
	{
		if (OtherActor != this && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(MoveComp->Velocity * 100.0f, Hit.ImpactPoint);

			UHealthComponent* HealthComp = Cast<UHealthComponent>(OtherActor->GetComponentByClass(UHealthComponent::StaticClass()));
			if (HealthComp)
			{
				HealthComp->ApplyHealthChange(-30.0f);
				UE_LOG(LogTemp, Warning, TEXT("Take Damage by BaseProjectile!!!"));
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Take Damage by BaseProjectile!!!"));
			}
		}
		if (OtherActor != GetInstigator())
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Instigator is not same with attacked one!"));
		}
	}
}

void ABaseProjectile::Explode_Implementation()
{
	// Check to make sure we aren't already being 'destroyed'
	// Adding ensure to see if we encounter this situation at all
	if (ensure(IsValid(this)))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());

		UGameplayStatics::PlayWorldCameraShake(this, CameraShakeStruct.ImpactShake, GetActorLocation(), CameraShakeStruct.ImpactShakeInnerRadius, CameraShakeStruct.ImpactShakeOuterRadius);

		//Destroy();
	}
}

void ABaseProjectile::PostInitializeComponents()
{

	Super::PostInitializeComponents();
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);

	// More consistent to bind here compared to Constructor which may fail to bind if Blueprint was created before adding this binding (or when using hot reload)
	// PostInitializeComponent is the preferred way of binding any events.
	// Bind OnActorHit event on the OnComponentHit delegate
	SphereComp->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnActorHit);
}

