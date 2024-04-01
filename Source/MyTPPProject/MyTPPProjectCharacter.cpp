// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyTPPProjectCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "HealthComponent.h"
#include "Animation\AnimMontage.h"
#include "Animation\AnimInstance.h"
#include "PowerComponent.h"
#include "PropInteractComponent.h"
#include "DashProjectile.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);
DEFINE_LOG_CATEGORY_STATIC(TPPCharacterLog, All, All);

//////////////////////////////////////////////////////////////////////////
// AMyTPPProjectCharacter

AMyTPPProjectCharacter::AMyTPPProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and animation blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	//Create Health component for the character, bind health initialization function when health changed
	TPPHealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	TPPHealthComponent->OnHealthChanged.AddDynamic(this, &AMyTPPProjectCharacter::OnHealthChangeFunc);

	//Create Power component for the character, bind power initialization function when health changed
	TPPPowerComponent = CreateDefaultSubobject<UPowerComponent>(TEXT("PowerComp"));
	TPPPowerComponent->OnPowerChanged.AddDynamic(this, &AMyTPPProjectCharacter::OnPowerChangeFunc);

	//Create interact component for the character
	WuKongInteractComponent = CreateDefaultSubobject<UPropInteractComponent>(TEXT("InteractComp"));
}

void AMyTPPProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context (Enhanced input context system)
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//Bind death function after broadcasting OnDeath delegate
	TPPHealthComponent->OnDeath.AddUObject(this,&AMyTPPProjectCharacter::WuKongOnDeath);
}

//After finishing normal attack, execute this function to reset normal attack boolean value, then we can normal attack again
void AMyTPPProjectCharacter::SetIsNormalAttack()
{
	IsNormalAttack = false;
}

//log when health changed
void AMyTPPProjectCharacter::OnHealthChangeFunc(AActor* InstigatorActor, UHealthComponent* OwningComp, float NewHealth, float Delta)
{
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("%s"), *GetNameSafe(InstigatorActor)));
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("%s"), *GetNameSafe(OwningComp)));
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Health: %f"), NewHealth)); 
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("DeltaHealth: %f"), Delta));
		UE_LOG(TPPCharacterLog, Warning, TEXT("Health: %f"), Delta);
	}
}

//log when power changed
void AMyTPPProjectCharacter::OnPowerChangeFunc(AActor* InstigatorActor, UPowerComponent* OwningComp, float NewPower, float Delta)
{
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("%s"), *GetNameSafe(InstigatorActor)));
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("%s"), *GetNameSafe(OwningComp)));
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Power: %f"), NewPower));
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("DeltaPower: %f"), Delta));
		UE_LOG(TPPCharacterLog, Warning, TEXT("DeltaPower: %f"), Delta);
	}
}

//Death function
void AMyTPPProjectCharacter::WuKongOnDeath()
{
	UE_LOG(TPPCharacterLog, Error, TEXT("Player %s is dead!"), *GetNameSafe(this));
	PlayAnimMontage(DeathAnim);

	GetCharacterMovement()->DisableMovement();

	SetLifeSpan(5.0f);

	Controller->ChangeState(NAME_Spectating);
}

void AMyTPPProjectCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//TPPHealthComponent->OnHealthChanged.AddDynamic(this, &AMyTPPProjectCharacter::OnHealthChangeFunc);
	//TPPPowerComponent->OnPowerChanged.AddDynamic(this, &AMyTPPProjectCharacter::OnPowerChangeFunc);
}

void AMyTPPProjectCharacter::PrimaryInteract()
{
	if (WuKongInteractComponent)
	{
		WuKongInteractComponent->PrimaryInteract();
	}
}

//Spawn teleport projectile
void AMyTPPProjectCharacter::WuKongTeleport()
{
	if (DashProj)
	{
		FRotator MuzzleRotation;
		FVector MuzzleLocation;
		GetActorEyesViewPoint(MuzzleLocation, MuzzleRotation);
		MuzzleRotation = GetControlRotation();
		MuzzleLocation = MuzzleLocation + MuzzleRotation.Vector() * 300;
		FTransform const SpawnTM = FTransform(MuzzleRotation, MuzzleLocation);

		UWorld* World = GetWorld();
		if (!World)
		{
			return;
		}
		int currentpower = TPPPowerComponent->GetPower();
		if (currentpower >= 50.0f)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			PlayAnimMontage(TeleportMontage);

			//ADashProjectile* DashProjectile = nullptr;
			ADashProjectile* DashProjectile = World->SpawnActor<ADashProjectile>(DashProj, SpawnTM, SpawnParams);
			
			TPPPowerComponent->SetPower(currentpower - 50.0f);
			GetWorldTimerManager().SetTimer(TPPPowerComponent->PowerHealTimerHandle, this, &AMyTPPProjectCharacter::PowerHeal, 2.0f, true);
			
			MuzzleRotation.Pitch = 0;
			MuzzleRotation.Roll = 0;
			SetActorRotation(MuzzleRotation);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Power not Enough!")));
		}
	}
}

//Heal power after spawning teleport projectile
void AMyTPPProjectCharacter::PowerHeal() const
{
	TPPPowerComponent->PowerHealUpdate();
}


//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Input

void AMyTPPProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyTPPProjectCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyTPPProjectCharacter::Look);

		//NormalAttack
		EnhancedInputComponent->BindAction(NormalAttackAction,ETriggerEvent::Started, this, &AMyTPPProjectCharacter::NormalAttack);

		//ChargedAttack is bind in the Blueprint

		//Interact
		EnhancedInputComponent->BindAction(Interact_Prop, ETriggerEvent::Started, this, &AMyTPPProjectCharacter::PrimaryInteract);

		//Teleport
		EnhancedInputComponent->BindAction(TeleportProjectile,ETriggerEvent::Started, this, &AMyTPPProjectCharacter::WuKongTeleport);
	} 
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

//Base operate for moving
void AMyTPPProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Cyan, FString::Printf(TEXT("The current controller YawRotation is: %s"), *YawRotation.ToString()));

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Cyan, FString::Printf(TEXT("The current ForwardDirection is: %s"), *ForwardDirection.ToString()));
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Cyan, FString::Printf(TEXT("The current RightDirection is: %s"), *RightDirection.ToString()));

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

//Base operate for looking
void AMyTPPProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

