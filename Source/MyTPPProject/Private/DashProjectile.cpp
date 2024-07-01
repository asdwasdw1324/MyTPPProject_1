// Fill out your copyright notice in the Description page of Project Settings.


#include "DashProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ADashProjectile::ADashProjectile()
{
	TeleportDelay = 0.5f;
	DetonateDelay = 0.5f;

	MoveComp->InitialSpeed = 5000.0f;
}

void ADashProjectile::BeginPlay()
{
	Super::BeginPlay();

	//Set explode timer after launch
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ADashProjectile::Explode, DetonateDelay);
	
}

void ADashProjectile::Explode_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);
	
	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(this, EmitterSound, GetActorLocation());

	EffectComp->DeactivateSystem();

	MoveComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	//Start teleport timer after explode
	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ADashProjectile::TeleportInstigator, TeleportDelay);

	// Skip base implementation as it will destroy actor (we need to stay alive a bit longer to finish the 2nd timer)
	//Super::Explode_Implementation();
}

void ADashProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport))
	{
		//Avoid the teleport location is in the air
		FVector actorlocation = GetActorLocation();
		FVector TeleportLocation = FVector(actorlocation.X, actorlocation.Y, 0);
		
		// Keep instigator rotation or it may end up jarring
		ActorToTeleport->TeleportTo(TeleportLocation, ActorToTeleport->GetActorRotation(), false, false);

		// Play shake on the player we teleported
		APawn* InstigatorPawn = Cast<APawn>(ActorToTeleport);
		APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());
		if (PC && PC->IsLocalController())
		{
			PC->ClientStartCameraShake(CameraShakeStruct.ImpactShake);
		}
	}
	// Now we're ready to destroy self
	Destroy();
}

