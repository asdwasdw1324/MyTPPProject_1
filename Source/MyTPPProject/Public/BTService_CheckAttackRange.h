// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckAttackRange.generated.h"

/**
 * 
 */

class UBlackboardComponent;
class AAIController;

UCLASS()
class MYTPPPROJECT_API UBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CheckAttackRange();	

protected:

	UPROPERTY(EditAnywhere, Category = "AIParameter")
	FBlackboardKeySelector AttackRangeKey;

	UPROPERTY(EditAnywhere, Category = "AIParameter")
	float MaxAttackRange;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
