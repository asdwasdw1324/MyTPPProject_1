// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckAttackRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_CheckAttackRange::UBTService_CheckAttackRange()
{
	MaxAttackRange = 2000.0f;

	bNotifyBecomeRelevant = true;

}

void UBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//Purpose is to check distance between AIPawn and TargetActor(PlayerPawn) and give this bool value to a blackboard key
	
	//OwnerComp is the BehaviorTree instance which includes current BTService
	//Get BlackBoard Asset associated with current BehaviorTree instance
	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject("CharacterLocation"));
		if (ensure(TargetActor))
		{
			AAIController* MyAIController = OwnerComp.GetAIOwner();
			if (ensure(MyAIController))
			{
				APawn* AIPawn = MyAIController->GetPawn();

				if (ensure(AIPawn))
				{	
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					bool bWithinRange = DistanceTo < MaxAttackRange;

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

	APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (ensure(MyPawn))
	{
		AAIController* MyAIController = Cast<AAIController>(OwnerComp.GetAIOwner());
		if (MyAIController)
		{
			MyAIController->GetBlackboardComponent()->SetValueAsObject("CharacterLocation", MyPawn);
			MyAIController->GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
		}
	}

}