// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MyTPPProjectCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UHealthComponent;
class UAnimMontage;
class UPowerComponent;
class UPropInteractComponent;
class ADashProjectile;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AMyTPPProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* NormalAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TeleportProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Interact_Prop;


public:
	AMyTPPProjectCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/*Called for Normal Attack, Implement in the Blueprint*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Attack)
	void NormalAttack();

	/*Normal attack montage*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
	TObjectPtr<UAnimMontage> NorAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Teleport)
	TObjectPtr<UAnimMontage> TeleportMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = Attack)
	bool IsNormalAttack = false;

	UFUNCTION(BlueprintCallable, Category = Attack)
	void SetIsNormalAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage>DeathAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ADashProjectile>DashProj;


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION()
	void OnHealthChangeFunc(AActor* InstigatorActor, UHealthComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnPowerChangeFunc(AActor* InstigatorActor, UPowerComponent* OwningComp, float NewPower, float Delta);

	void WuKongOnDeath();
	
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = Attribute)
	TObjectPtr<UPowerComponent> TPPPowerComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = Attribute)
	TObjectPtr<UHealthComponent> TPPHealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = Interact)
	TObjectPtr<UPropInteractComponent> WuKongInteractComponent;

	UFUNCTION()
	void PrimaryInteract();

	UFUNCTION(BlueprintCallable)
	void WuKongTeleport();

	UFUNCTION()
	void PowerHeal();
	
};

