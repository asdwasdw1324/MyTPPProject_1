// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Attack.generated.h"

/**
 * 
 */

class ABaseProjectile;
class AAIController;
class ACharacter;

UCLASS()
class MYTPPPROJECT_API UBTTaskNode_Attack : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:

	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<ABaseProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	UAnimMontage* AttackAnimation;
};
