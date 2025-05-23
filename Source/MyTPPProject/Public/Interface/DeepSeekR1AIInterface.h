<<<<<<< Updated upstream
// #pragma once
//
// #include "CoreMinimal.h"
// #include "GameFramework/Actor.h"
// #include "Interface/DeepSeekR1Interface.h"
// #include "Components/SphereComponent.h"
// #include "Components/SkeletalMeshComponent.h"
// #include "Components/WidgetComponent.h"
// #include "DeepSeekR1AIInterface.generated.h"
//
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIResponseReceived, const FString&, Response);
//
// USTRUCT(BlueprintType)
// struct FDeepSeekResponse
// {
//     GENERATED_BODY()
//
//     UPROPERTY(BlueprintReadOnly, Category = "DeepSeek R1 AI")
//     FString Response;
//
//     UPROPERTY(BlueprintReadOnly, Category = "DeepSeek R1 AI")
//     bool Success;
// };
//
// // 对话历史结构
// USTRUCT(BlueprintType)
// struct FConversationEntry
// {
//     GENERATED_BODY()
//     
//     FString Role;    // "user" 或 "assistant"
//     FString Content; // 对话内容
// };
//
// UCLASS()
// class ADeepSeekR1AIInterface : public AActor, public IDeepSeekR1Interface
// {
//     GENERATED_BODY()
//
// public:
//     ADeepSeekR1AIInterface();
//
//     // Implementation of IDeepSeekR1Interface
//     UFUNCTION(BlueprintCallable) // 允许在蓝图中调用
//     virtual void AskQuestion(const FString& Question, AActor* ContextActor = nullptr) override;
//     
//     UFUNCTION(BlueprintCallable) // 允许在蓝图中调用
//     virtual void AnalyzeScene(AActor* TargetActor = nullptr) override;
//
//     // Event dispatcher for when AI response is received
//     UPROPERTY(BlueprintAssignable, Category = "DeepSeek R1 AI")
//     FOnAIResponseReceived OnAIResponseReceived;
//
//     // 交互范围
//     UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DeepSeek R1 AI")
//     USphereComponent* InteractionSphere;
//
//     // 机器人模型
//     UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DeepSeek R1 AI")
//     USkeletalMeshComponent* RobotMesh;
//
//     // UI组件
//     UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DeepSeek R1 AI")
//     UWidgetComponent* InteractionWidget;
//
//     // 交互函数
//     UFUNCTION()
//     void OnInteractionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
//         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
//
//     UFUNCTION()
//     void OnInteractionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
//         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
//
//     // 显示/隐藏交互UI
//     UFUNCTION(BlueprintCallable, Category = "DeepSeek R1 AI")
//     void ShowInteractionWidget();
//
//     UFUNCTION(BlueprintCallable, Category = "DeepSeek R1 AI")
//     void HideInteractionWidget();
//
//     UFUNCTION(BlueprintCallable, Category = "Movement")
//     void MoveLogic();
//
//     void AddHistoryEntry(const FString& Role, const FString& Content);
//
// protected:
//     virtual void BeginPlay() override;
//
//     virtual void Tick(float DeltaTime) override;
//
//     // Internal function to process AI responses
//     void ProcessAIResponse(const FString& Response);
//
//     // Function to get scene context information
//     FString GetSceneContext(AActor* ContextActor = nullptr);
//
// private:
//     // HTTP client for API communication
//     TSharedPtr<class IHttpRequest> HttpRequest;
//
//     // API endpoint configuration
//     UPROPERTY(EditDefaultsOnly, Category = "DeepSeek R1 AI", meta = (AllowPrivateAccess = "true"))
//     FString ApiEndpoint;
//
//     UPROPERTY(EditDefaultsOnly, Category = "DeepSeek R1 AI", meta = (AllowPrivateAccess = "true"))
//     FString ApiKey;
//
//     TArray<FConversationEntry> ConversationHistory; // 对话历史记录
//     
//     FString CurrentContext; // 当前场景上下文
//
//     // 当前是否有玩家在交互范围内
//     bool bIsPlayerInRange;
//
//     // Rob的初始位置
//     FVector RobInitialLocation;
//
//     //初始化组件
//     virtual void PostInitializeComponents() override;
// };
=======
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/DeepSeekR1Interface.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "DeepSeekR1AIInterface.generated.h"

//回答结构体包含回答内容和是否成功
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
    void OnInteractionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnInteractionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

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
>>>>>>> Stashed changes
