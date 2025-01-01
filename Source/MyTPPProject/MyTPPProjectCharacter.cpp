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
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "PowerComponent.h"
#include "PropInteractComponent.h"
#include "DashProjectile.h"
#include "DataAsset/DataAsset_InputConfig.h"
#include "WuKongEnhancedInputComponent.h"
#include "GamePlayTags/WuKongGamePlayTags.h"
#include "AbilitySystem/WuKongAttributeSet.h"
#include "AbilitySystem/WuKongAbilitySystemComponent.h"
#include "DataAsset/DataAsset_StartUpDataBase.h"
#include "Blueprint\UserWidget.h"
#include "InputMappingContext.h"
#include "Component/WuKongCombatComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);
DEFINE_LOG_CATEGORY(LogWuKongCharacter);
DEFINE_LOG_CATEGORY(LogWuKongAbility);
DEFINE_LOG_CATEGORY_STATIC(TPPCharacterLog, All, All);

//////////////////////////////////////////////////////////////////////////
// AMyTPPProjectCharacter

AMyTPPProjectCharacter::AMyTPPProjectCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	// 优化网络更新
	NetUpdateFrequency = 60.0f;
	MinNetUpdateFrequency = 30.0f;
	// 优化碰撞设置
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	CurrentState = EWuKongCharacterState::Alive;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetMesh()->bReceivesDecals = false;
	GetMesh()->bCastDynamicShadow = false;
	GetMesh()->CastShadow = false;

	// Configure character movement
	// Character moves in the direction of input...
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// ...at this rotation rate
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//FollowCamera->SetupAttachment(CameraBoom, TEXT("None"));
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and animation blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	//Create Health component for the character, bind health initialization function when health changed
	TppHealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	//TppHealthComponent->OnHealthChanged.AddDynamic(this, &AMyTPPProjectCharacter::OnHealthChangeFunc);
	/** Bind Death function after broadcasting OnDeath delegate*/
	//TppHealthComponent->OnDeath.AddUObject(this, &AMyTPPProjectCharacter::WuKongOnDeath);

	//Create Power component for the character, bind power initialization function when power changed
	TppPowerComponent = CreateDefaultSubobject<UPowerComponent>(TEXT("PowerComp"));
	//TppPowerComponent->OnPowerChanged.AddDynamic(this, &AMyTPPProjectCharacter::OnPowerChangeFunc);

	//Create Interact component for the character
	WuKongInteractComponent = CreateDefaultSubobject<UPropInteractComponent>(TEXT("InteractComp"));

	//Create AbilitySystem and AttributeSet component for the character
	WuKongAbilitySystemComponent = CreateDefaultSubobject<UWuKongAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	WuKongAttributeSet = CreateDefaultSubobject<UWuKongAttributeSet>(TEXT("AttributeSet"));

	//Create Combat component for the character
	WuKongCombatComponent = CreateDefaultSubobject<UWuKongCombatComponent>(TEXT("CombatComp"));
}

UAbilitySystemComponent* AMyTPPProjectCharacter::GetAbilitySystemComponent() const
{
	return GetWuKongAbilitySystemComponent();
}

void AMyTPPProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
}

void AMyTPPProjectCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (WuKongAbilitySystemComponent)
	{
		WuKongAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	
}

//After finishing normal attack, execute this function to reset normal attack boolean value, then we can do normal attack again
void AMyTPPProjectCharacter::SetIsNormalAttack(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == NorAttackMontage && !bInterrupted)
	{
		IsNormalAttack = false;
	}
}

//log when health changed
void AMyTPPProjectCharacter::OnHealthChangeFunc(AActor* InstigatorActor, UHealthComponent* OwningComp, float NewHealth, float Delta)
{
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("%s"), *GetNameSafe(InstigatorActor)));
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("%s"), *GetNameSafe(OwningComp)));
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Health: %f"), NewHealth)); 
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("DeltaHealth: %f"), Delta));
		UE_LOG(LogWuKongCharacter, Warning, TEXT("Health: %f, DeltaHealth: %f"), NewHealth, Delta);
	}
}

//log when power changed
void AMyTPPProjectCharacter::OnPowerChangeFunc(AActor* InstigatorActor, UPowerComponent* OwningComp, float NewPower, float Delta)
{
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *GetNameSafe(InstigatorActor)));
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *GetNameSafe(OwningComp)));
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Power: %f"), NewPower));
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("DeltaPower: %f"), Delta));
		UE_LOG(LogWuKongCharacter, Warning, TEXT("Power: %f, DeltaPower: %f"), NewPower, Delta);
	}
}

//Death function
void AMyTPPProjectCharacter::WuKongOnDeath()
{
	if (CurrentState == EWuKongCharacterState::Dead)
	{
		return;
	}
	CurrentState = EWuKongCharacterState::Dead;
	OnCharacterStateChanged.Broadcast(CurrentState);

	UE_LOG(TPPCharacterLog, Error, TEXT("Player %s is dead!"), *GetNameSafe(this));

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (DeathAnim)
	{
		float AnimLength = DeathAnim->GetPlayLength();
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Death anim length: %f"), AnimLength));
		
		if (AnimInstance)
		{
			USkeletalMesh* CharacterMesh = GetMesh()->GetSkeletalMeshAsset();
			if (CharacterMesh && DeathAnim->GetSkeleton() != CharacterMesh->GetSkeleton())
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Death animation skeleton mismatch!"));
				return;
			}

			AnimInstance->Montage_Play(DeathAnim);
		}
	}

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(3.0f);
	
}

void AMyTPPProjectCharacter::WuKongOnStateChanged(EWuKongCharacterState NewState)
{
	if (NewState == EWuKongCharacterState::Dead)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				Subsystem->ClearAllMappings();
			}
		
			FInputModeUIOnly InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputMode);
			ShowDeathUI();
			PC->bShowMouseCursor = true;

			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("Please select an option to continue!!!")));

			//切换到观察者模式（如果需要）
			// if (Controller)
			// {
			//      Controller->ChangeState(NAME_Spectating);
			// }
		}
	}
	if (NewState == EWuKongCharacterState::Alive)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Player %s has come back! Game restart!"), *GetNameSafe(this)));
	}
}

void AMyTPPProjectCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	TppHealthComponent->OnHealthChanged.AddDynamic(this, &AMyTPPProjectCharacter::OnHealthChangeFunc);
	TppHealthComponent->OnDeath.AddUObject(this, &AMyTPPProjectCharacter::WuKongOnDeath);
	TppPowerComponent->OnPowerChanged.AddDynamic(this, &AMyTPPProjectCharacter::OnPowerChangeFunc);
	OnCharacterStateChanged.AddDynamic(this, &AMyTPPProjectCharacter::WuKongOnStateChanged);
}

void AMyTPPProjectCharacter::WuKongNormalAttack()
{
	if (!IsNormalAttack)
	{
		if (TppPowerComponent->Power >= 5.0f)
		{
			IsNormalAttack = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (NorAttackMontage && AnimInstance)
			{
				if (AnimInstance->OnMontageEnded.IsBound())
				{
					AnimInstance->OnMontageEnded.RemoveDynamic(this, &AMyTPPProjectCharacter::SetIsNormalAttack);
				}
				AnimInstance->OnMontageEnded.AddDynamic(this, &AMyTPPProjectCharacter::SetIsNormalAttack);
				AnimInstance->Montage_Play(NorAttackMontage);
				JudgePowerHealTimerHandleRunning();
				TppPowerComponent->SetPower(TppPowerComponent->Power - 5.0f);
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("PowerNotEnoughForNormalAttack!!!")));
		}
	}
}

void AMyTPPProjectCharacter::JudgePowerHealTimerHandleRunning()
{
	bool bIsTimerActive = GetWorld()->GetTimerManager().IsTimerActive(TppPowerComponent->PowerHealTimerHandle);

	if (!bIsTimerActive)
	{
		GetWorld()->GetTimerManager().SetTimer(TppPowerComponent->PowerHealTimerHandle, this, &AMyTPPProjectCharacter::PowerHeal, 2.0f, true);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("PowerisHealing!!!")));
	}
}

void AMyTPPProjectCharacter::ActiveEnhancedAttackStatus()
{
	if (WuKongAbilitySystemComponent)
	{
		if (!CharacterStartUpData.IsNull())
		{
			if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
			{
				LoadedData->GiveToAbilitySystemComponent(WuKongAbilitySystemComponent);
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, FString::Printf(TEXT("CharacterData is null! %s"), *GetName()));
		}
	}
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
		//Confirm DashProj launch point
		FRotator MuzzleRotation;
		FVector MuzzleLocation;
		GetActorEyesViewPoint(MuzzleLocation, MuzzleRotation);
		MuzzleRotation = GetControlRotation();
		MuzzleLocation = MuzzleLocation + MuzzleRotation.Vector() * 300;
		FTransform SpawnTM = FTransform(MuzzleRotation, MuzzleLocation);

		//Confirm world existed, or will not spawn actor
		UWorld* World = GetWorld();
		if (!World)
		{
			return;
		}

		int currentpower = TppPowerComponent->GetPower();
		if (currentpower >= 50.0f)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			PlayAnimMontage(TeleportMontage);

			//ADashProjectile* DashProjectile = nullptr;
			ADashProjectile* DashProjectile = World->SpawnActor<ADashProjectile>(DashProj, SpawnTM, SpawnParams);
			
			TppPowerComponent->SetPower(currentpower - 50.0f);

			bool bIsPowerHealTimerHandleActive = GetWorldTimerManager().IsTimerActive(TppPowerComponent->PowerHealTimerHandle);
			if (!bIsPowerHealTimerHandleActive)
			{
				GetWorldTimerManager().SetTimer(TppPowerComponent->PowerHealTimerHandle, this, &AMyTPPProjectCharacter::PowerHeal, 2.0f, true);
			}
			
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

//Heal power after attacking/teleport(doing anything consume power)
void AMyTPPProjectCharacter::PowerHeal()
{
	if (TppPowerComponent)
	{
		TppPowerComponent->PowerHealUpdate();
	}
	
}


// RegularInput
void AMyTPPProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Add Input Mapping Context (Enhanced input context system)
	//Get the player controller
	if (APlayerController* PlayerController = GetController<APlayerController>())
	{
		//Get the local player and get the enhanced input local player subsystem from it
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UWuKongEnhancedInputComponent* WuKongEnhancedInputComponent = CastChecked<UWuKongEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping(Using default binding function for testing and distinguishing difference)
		WuKongEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		WuKongEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		WuKongEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, WuKongGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &AMyTPPProjectCharacter::Move);

		// Looking
		WuKongEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, WuKongGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &AMyTPPProjectCharacter::Look);

		//NormalAttack
		WuKongEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, WuKongGameplayTags::InputTag_NormalAttack, ETriggerEvent::Started, this, &AMyTPPProjectCharacter::WuKongNormalAttack);

		//ChargedAttack
		WuKongEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, WuKongGameplayTags::InputTag_ChargedAttack, ETriggerEvent::Triggered, this, &AMyTPPProjectCharacter::ChargedAttack_Triggered);
		WuKongEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, WuKongGameplayTags::InputTag_ChargedAttack, ETriggerEvent::Ongoing, this, &AMyTPPProjectCharacter::ChargedAttack_Ongoing);
		WuKongEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, WuKongGameplayTags::InputTag_ChargedAttack, ETriggerEvent::Canceled, this, &AMyTPPProjectCharacter::ChargedAttack_Cancled);

		//Interact
		WuKongEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, WuKongGameplayTags::InputTag_Interact, ETriggerEvent::Started, this, &AMyTPPProjectCharacter::PrimaryInteract);

		//Teleport
		WuKongEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, WuKongGameplayTags::InputTag_Teleport, ETriggerEvent::Started, this, &AMyTPPProjectCharacter::WuKongTeleport);

		//EnhancedAttackDamage
		WuKongEnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, WuKongGameplayTags::InputTag_EnhancedAttack, ETriggerEvent::Completed, this, &AMyTPPProjectCharacter::ActiveEnhancedAttackStatus);
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
		FRotator Rotation = Controller->GetControlRotation();
		Rotation.Pitch = 0;
		Rotation.Roll = 0;
		FRotator YawRotation(Rotation.Pitch, Rotation.Yaw, Rotation.Roll);
		//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Cyan, FString::Printf(TEXT("The current controller YawRotation is: %s"), *YawRotation.ToString()));

		// get forward vector
		FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Orange, FString::Printf(TEXT("The current FRotationMatrix is: %s"), *FRotationMatrix(YawRotation).ToString()));
		//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Orange, FString::Printf(TEXT("The current ForwardDirection is: %s"), *ForwardDirection.ToString()));
		//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Cyan, FString::Printf(TEXT("The current ForwardDirection is: %s"), *ForwardDirection.ToString()));
	
		// get right vector 
		FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
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
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
// RegularInput

void AMyTPPProjectCharacter::RestartGameAfterDeath()
{
	CurrentState = EWuKongCharacterState::Alive;
	OnCharacterStateChanged.Broadcast(CurrentState);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->RestartLevel();
		PC->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		InputMode.SetConsumeCaptureMouseDown(true);
		PC->SetInputMode(InputMode);
		// UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)));
	}
}

void AMyTPPProjectCharacter::ShowDeathUI()
{
	if (WuKongDeathUI)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			UUserWidget* DeathWidget = CreateWidget<UUserWidget>(PC, WuKongDeathUI);
			if (DeathWidget)
			{
				DeathWidget->AddToViewport();
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("DeathUI is not set!"));
	}
}

