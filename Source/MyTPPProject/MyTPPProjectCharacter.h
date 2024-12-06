// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "MyTPPProjectCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UHealthComponent;
class UAnimMontage;
class UPowerComponent;
class UPropInteractComponent;
class ADashProjectile;
class UWidgetComponent;
class UDataAsset_InputConfig;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

class UWuKongAttributeSet;
class UWuKongAbilitySystemComponent;

UCLASS(config=Game)
class AMyTPPProjectCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputMappingContext* DefaultMappingContext;

	/** Jumping Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputAction* MoveAction;

	/** Look Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputAction* LookAction;

	/** NormalAttack Input Action */
	//UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputAction* NormalAttackAction;

	/** Teleport Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputAction* TeleportProjectile;

	/** Interact Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputAction* Interact_Prop;

	/** ChargedAttack Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputAction* ChargedAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;


public:
	AMyTPPProjectCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	//PowerComponent
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = Attribute)
	TObjectPtr<UPowerComponent> TppPowerComponent;

	//HealthComponent
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = Attribute)
	TObjectPtr<UHealthComponent> TppHealthComponent;

	//InteractComponent
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = Interact)
	TObjectPtr<UPropInteractComponent> WuKongInteractComponent;

	//AbilitySystemComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilitySystem)
	TObjectPtr<UWuKongAbilitySystemComponent> WuKongAbilitySystemComponent;

	//AttributeSetComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilitySystem)
	TObjectPtr<UWuKongAttributeSet> WuKongAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = Attack)
	bool IsNormalAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = Attack)
	bool CanTriggerAttack;

	UFUNCTION(BlueprintCallable, Category = Attack)
	void SetIsNormalAttack(UAnimMontage* Montage, bool bInterrupted);

protected:

#pragma region Inputs
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/*Called for Normal Attack, Implement in the Blueprint*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Attack)
	void NormalAttack();

	UFUNCTION(BlueprintCallable)
	bool WuKongNormalAttack();

	/*Called for Charged Attack, Implement in the Blueprint*/
	UFUNCTION(BlueprintImplementableEvent, Category = Attack)
	void ChargedAttack_Triggered();

	UFUNCTION(BlueprintImplementableEvent, Category = Attack)
	void ChargedAttack_Ongoing();

	UFUNCTION(BlueprintImplementableEvent, Category = Attack)
	void ChargedAttack_Cancled();

	/** Called for PrimaryInteract input */
	UFUNCTION(BlueprintCallable)
	void PrimaryInteract();

	/** Called for WuKongTeleport input */
	UFUNCTION(BlueprintCallable)
	void WuKongTeleport();
#pragma endregion
	
	/*Normal attack montage*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<UAnimMontage> NorAttackMontage;

	/*Teleport montage*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<UAnimMontage> TeleportMontage;

	/*Death montage*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<UAnimMontage>DeathAnim;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<ADashProjectile>DashProj;
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;

	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

public:
	/** Returns CameraBoom subObject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subObject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns AbilitySystemComponent subObject **/
	FORCEINLINE UWuKongAbilitySystemComponent* GetWuKongAbilitySystemComponent() const {return WuKongAbilitySystemComponent;}
	/** Returns AttributeSet subObject **/
	FORCEINLINE UWuKongAttributeSet* GetWuKongWuKongAttributeSet() const {return WuKongAttributeSet;}

	UFUNCTION()
	void OnHealthChangeFunc(AActor* InstigatorActor, UHealthComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnPowerChangeFunc(AActor* InstigatorActor, UPowerComponent* OwningComp, float NewPower, float Delta);

	//static single broadcast
	void WuKongOnDeath();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerState)
	bool IsDeath;
	
	UFUNCTION(BlueprintCallable)
	void PowerHeal();
	
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable)
	bool ConsumePowerAfterTrigger(float ConsumePower);

	UFUNCTION(BlueprintCallable)
	void JudgePowerHealTimerHandleRunning();

// private:
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, meta=(AllowPrivateAccess = "true"))
// 	UWidgetComponent* ChargingProgressWidget;
	
};

