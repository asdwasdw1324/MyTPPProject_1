// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Attack.h"
#include "AIController.h"
#include "BaseProjectile.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyTPPProject/MyTPPProjectCharacter.h"

EBTNodeResult::Type UBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* MyController = OwnerComp.GetAIOwner())
	{
		ACharacter* MyPawn = Cast<AMyTPPProjectCharacter>(MyController->GetPawn());

		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("hand_r");

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("EnemyActor"));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();
		FVector FinalMuzzleLocation = MuzzleLocation + MuzzleRotation.Vector() * 100;

		FActorSpawnParameters Params;
		Params.Owner = MyPawn;
		Params.Instigator = MyPawn;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FCollisionQueryParams AIQueryParams;
		AIQueryParams.AddIgnoredActor(MyPawn);

		FCollisionObjectQueryParams AIObjectParams;
		AIObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		AIObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
		AIObjectParams.AddObjectTypesToQuery(ECC_Pawn);

		FTransform AISpawnTM = FTransform(MuzzleRotation, FinalMuzzleLocation);

		ABaseProjectile* NewProj = nullptr;
		if (ensure(ProjectileClass))
		{
			ACharacter* TargetCharacter = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("CharacterLocation"));
			check(AttackAnimation);
			TargetCharacter->PlayAnimMontage(AttackAnimation);

			NewProj = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, AISpawnTM, Params);
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("NewProjName: %s"), *GetNameSafe(NewProj)));
		}
		return NewProj ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Failed;
}
