#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/DeepSeekR1Interface.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "DeepSeekR1AIInterface.generated.h"

USTRUCT(BlueprintType)
struct FDeepSeekResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "DeepSeek R1 AI")
    FString Response;

    UPROPERTY(BlueprintReadOnly, Category = "DeepSeek R1 AI")
    bool Success;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIResponseReceived, const FString&, Response);

UCLASS()
class ADeepSeekR1AIInterface : public AActor, public IDeepSeekR1Interface
{
    GENERATED_BODY()

public:
    ADeepSeekR1AIInterface();

    // Implementation of IDeepSeekR1Interface
    virtual void AskQuestion(const FString& Question, AActor* ContextActor = nullptr) override;
    virtual void AnalyzeScene(AActor* TargetActor = nullptr) override;

    // Event dispatcher for when AI response is received
    UPROPERTY(BlueprintAssignable, Category = "DeepSeek R1 AI")
    FOnAIResponseReceived OnAIResponseReceived;

    // 交互范围
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DeepSeek R1 AI")
    USphereComponent* InteractionSphere;

    // 机器人模型
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DeepSeek R1 AI")
    USkeletalMeshComponent* RobotMesh;

    // UI组件
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DeepSeek R1 AI")
    UWidgetComponent* InteractionWidget;

    // 交互函数
    UFUNCTION()
    void OnInteractionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnInteractionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // 显示/隐藏交互UI
    UFUNCTION(BlueprintCallable, Category = "DeepSeek R1 AI")
    void ShowInteractionWidget();

    UFUNCTION(BlueprintCallable, Category = "DeepSeek R1 AI")
    void HideInteractionWidget();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveLogic();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Internal function to process AI responses
    void ProcessAIResponse(const FString& Response);

    // Function to get scene context information
    FString GetSceneContext(AActor* ContextActor = nullptr);

private:
    // HTTP client for API communication
    TSharedPtr<class IHttpRequest> HttpRequest;

    // API endpoint configuration
    UPROPERTY(EditDefaultsOnly, Category = "DeepSeek R1 AI", meta = (AllowPrivateAccess = "true"))
    FString ApiEndpoint;

    UPROPERTY(EditDefaultsOnly, Category = "DeepSeek R1 AI", meta = (AllowPrivateAccess = "true"))
    FString ApiKey;

    // 当前是否有玩家在交互范围内
    bool bIsPlayerInRange;

    FVector RobInitialLocation;
};
