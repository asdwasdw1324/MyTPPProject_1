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
class UHealthComponent;
class UAnimMontage;
class UPowerComponent;
class UPropInteractComponent;
class ADashProjectile;
struct FInputActionValue;

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

	/** NormalAttack Input Action */
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* NormalAttackAction;

	/** Teleport Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TeleportProjectile;

	/** Interact Input Action */
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

	/** Called for PrimaryInteract input */
	UFUNCTION(BlueprintCallable)
	void PrimaryInteract();

	/** Called for WuKongTeleport input */
	UFUNCTION(BlueprintCallable)
	void WuKongTeleport();

	/*Normal attack montage*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
	TObjectPtr<UAnimMontage> NorAttackMontage;

	/*Teleport montage*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Teleport)
	TObjectPtr<UAnimMontage> TeleportMontage;

	/*Death montage*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage>DeathAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = Attack)
	bool IsNormalAttack = false;

	UFUNCTION(BlueprintCallable, Category = Attack)
	void SetIsNormalAttack();

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

	UFUNCTION()
	void PowerHeal() const;
	
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = Attribute)
	TObjectPtr<UPowerComponent> TPPPowerComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = Attribute)
	TObjectPtr<UHealthComponent> TPPHealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = Interact)
	TObjectPtr<UPropInteractComponent> WuKongInteractComponent;
	
};

