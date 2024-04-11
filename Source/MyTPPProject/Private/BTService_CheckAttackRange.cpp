// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckAttackRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_CheckAttackRange::UBTService_CheckAttackRange()
{
	MaxAttackRange = 1000.0f;

	bNotifyBecomeRelevant = true;

}

void UBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//Purpose is to check distance between AIPawn and TargetActor(PlayerPawn) and give this bool value to a blackboard key
	
	//OwnerComp is the BehaviorTree instance which includes current BTService
	//Get BlackBoard Asset associated with current BehaviorTree instance
	if (UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent())
	{
		if (const AActor* TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject("EnemyActor")))
		{
			if (const AAIController* MyAIController = OwnerComp.GetAIOwner())
			{
				if (const APawn* AIPawn = MyAIController->GetPawn())
				{
					const float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					const bool bWithinRange = DistanceTo < MaxAttackRange;

					bool bHasLOS = false;
					if (bWithinRange)
					{
						bHasLOS = MyAIController->LineOfSightTo(TargetActor);
					}
					BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLOS));
				}
			}
		}
	}
}

void UBTService_CheckAttackRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		if (AAIController* MyAIController = Cast<AAIController>(OwnerComp.GetAIOwner()))
		{
			MyAIController->GetBlackboardComponent()->SetValueAsObject("EnemyActor", MyPawn);
			MyAIController->GetBlackboardComponent()->SetValueAsVector("PatrolLocation", MyPawn->GetActorLocation());
		}
	}

}