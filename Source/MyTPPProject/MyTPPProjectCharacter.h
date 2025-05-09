// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
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
class UDataAsset_StartUpDataBase;
class UWuKongAttributeSet;
class UWuKongAbilitySystemComponent;
class UUserWidget;
class UWuKongCombatComponent;
struct FInputActionValue;
struct FGameplayAbilitySpecHandle;

DECLARE_LOG_CATEGORY_EXTERN(LogCharacter, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogWuKongCharacter, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogWuKongAbility, Log, All);


UENUM(BlueprintType)
enum class EWuKongCharacterState : uint8
{
	Alive UMETA(DisplayName = "Alive"),
	Dead UMETA(DisplayName = "Dead"),
	Stunned UMETA(DisplayName = "Stunned"),
	// ... 其他状态
};

UCLASS(config=Game)
class AMyTPPProjectCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
	
	/** Internal Jumping Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataAsset_InputConfig> InputConfigDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CharacterData, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;


public:
	AMyTPPProjectCharacter();

	UPROPERTY(BlueprintReadWrite, Category = State)
	EWuKongCharacterState CurrentState;

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

	//WuKongCombatComponent
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CombatSystem)
	TObjectPtr<UWuKongCombatComponent> WuKongCombatComponent;

	//Can NormalAttack
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = Attack)
	bool IsNormalAttack;

	//Reset Can NormalAttack
	UFUNCTION(BlueprintCallable, Category = Attack)
	void SetIsNormalAttack(UAnimMontage* Montage, bool bInterrupted);

protected:

#pragma region Inputs
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/*C++ implementation for NormalAttack*/
	UFUNCTION(BlueprintCallable)
	void WuKongNormalAttack();

	/*Called for Normal Attack, Implement in the Blueprint*/
	//UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Attack)
	//void NormalAttack();

	/*Called for Charged Attack, Implement in the Blueprint*/
	UFUNCTION(BlueprintImplementableEvent, Category = Attack)
	void ChargedAttack_Triggered();

	UFUNCTION(BlueprintImplementableEvent, Category = Attack)
	void ChargedAttack_Ongoing();

	UFUNCTION(BlueprintImplementableEvent, Category = Attack)
	void ChargedAttack_Cancled();

	/** Called for active enhanced attack status */
	UFUNCTION(BlueprintCallable, Category = Attack)
	void ActiveEnhancedAttackStatus();

	/** Called for PrimaryInteract input */
	UFUNCTION(BlueprintCallable)
	void PrimaryInteract();

	/** Called for WuKongTeleport input */
	UFUNCTION(BlueprintCallable)
	void WuKongTeleport();

	/** Called for gameplay ability system input */
	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	TSubclassOf<UUserWidget> WuKongDeathUI;

public:
	/** Returns CameraBoom subObject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subObject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns AbilitySystemComponent subObject **/
	FORCEINLINE UWuKongAbilitySystemComponent* GetWuKongAbilitySystemComponent() const {return WuKongAbilitySystemComponent;}
	/** Returns AttributeSet subObject **/
	FORCEINLINE UWuKongAttributeSet* GetWuKongWuKongAttributeSet() const {return WuKongAttributeSet;}
	/** Returns HealthComponent subObject **/
	FORCEINLINE UHealthComponent* GetTppHealthComponent() const { return TppHealthComponent; }
	/** Returns PowerComponent subObject **/
	FORCEINLINE UPowerComponent* GetTppPowerComponent() const { return TppPowerComponent; }
	/** Returns PawnCombatComponent subObject **/
	FORCEINLINE UWuKongCombatComponent* GetWuKongCombatComponent() const { return WuKongCombatComponent; }
	
	UFUNCTION()
	void OnHealthChangeFunc(AActor* InstigatorActor, UHealthComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnPowerChangeFunc(AActor* InstigatorActor, UPowerComponent* OwningComp, float NewPower, float Delta);

	//static single broadcast
	void WuKongOnDeath();

	UFUNCTION()
	void WuKongOnStateChanged(EWuKongCharacterState NewState);

	//Automatic Power Heal function
	UFUNCTION(BlueprintCallable)
	void PowerHeal();

	//To bind the function to the delegation
	virtual void PostInitializeComponents() override;

	//Judge the PowerHealTimerHandle is running
	UFUNCTION(BlueprintCallable)
	void JudgePowerHealTimerHandleRunning();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterStateChanged, EWuKongCharacterState, NewState);
	
	UPROPERTY(BlueprintAssignable, Category = State)
	FOnCharacterStateChanged OnCharacterStateChanged;
	
	UFUNCTION(BlueprintCallable, Category = Game)
	void RestartGameAfterDeath();
	
	UFUNCTION(Category = UI)
	void ShowDeathUI();
	
};

