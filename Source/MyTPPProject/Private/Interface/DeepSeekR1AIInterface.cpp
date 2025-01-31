// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/DeepSeekR1AIInterface.h"
#include "HttpModule.h"
#include "Json.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet\GameplayStatics.h"
#include "MyTPPProject/MyTPPProjectCharacter.h"


ADeepSeekR1AIInterface::ADeepSeekR1AIInterface()
{
    PrimaryActorTick.bCanEverTick = true;
    ApiEndpoint = TEXT("https://api.deepseek.com/v1/chat"); // 替换为实际的DeepSeek API端点

    // 创建交互范围球体
    InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
    RootComponent = InteractionSphere;
    InteractionSphere->SetSphereRadius(200.0f);
    InteractionSphere->SetCollisionProfileName(TEXT("OverlapAll"));
    
    // 创建机器人模型
    RobotMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RobotMesh"));
    RobotMesh->SetupAttachment(RootComponent);
    
    // 创建UI组件
    InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
    InteractionWidget->SetupAttachment(RootComponent);
    InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen);
    InteractionWidget->SetDrawAtDesiredSize(true);
    InteractionWidget->SetVisibility(false);

    bIsPlayerInRange = false;
}

void ADeepSeekR1AIInterface::BeginPlay()
{
    Super::BeginPlay();
    
    // 绑定交互事件
    InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ADeepSeekR1AIInterface::OnInteractionSphereBeginOverlap);
    InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &ADeepSeekR1AIInterface::OnInteractionSphereEndOverlap);
}

void ADeepSeekR1AIInterface::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 这里可以添加一些动画效果或其他更新逻辑
}

void ADeepSeekR1AIInterface::AskQuestion(const FString& Question, AActor* ContextActor)
{
    if (Question.IsEmpty())
    {
        OnAIResponseReceived.Broadcast(TEXT("Question cannot be empty"));
        return;
    }

    // 获取场景上下文
    FString Context = GetSceneContext(ContextActor);
    
    // 构建请求内容
    TSharedPtr<FJsonObject> RequestObj = MakeShared<FJsonObject>();
    RequestObj->SetStringField(TEXT("question"), Question);
    RequestObj->SetStringField(TEXT("context"), Context);

    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(RequestObj.ToSharedRef(), Writer);

    // 创建HTTP请求
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequestPtr = FHttpModule::Get().CreateRequest();
    HttpRequestPtr->SetURL(ApiEndpoint);
    HttpRequestPtr->SetVerb(TEXT("POST"));
    HttpRequestPtr->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    HttpRequestPtr->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *ApiKey));
    HttpRequestPtr->SetContentAsString(RequestBody);
    HttpRequestPtr->SetTimeout(30.0f);  // 设置30秒超时

    // 设置回调
    HttpRequestPtr->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
    {
        if (bSuccess && Response.IsValid())
        {
            FString ResponseString = Response->GetContentAsString();
            ProcessAIResponse(ResponseString);
        }
        else
        {
            OnAIResponseReceived.Broadcast(TEXT("Failed to communicate with DeepSeek AI"));
        }
    });

    // 发送请求
    HttpRequestPtr->ProcessRequest();
}

void ADeepSeekR1AIInterface::AnalyzeScene(AActor* TargetActor)
{
    FString Context = GetSceneContext(TargetActor);
    FString Question = TEXT("Please analyze the current scene or object and provide insights.");
    AskQuestion(Question, TargetActor);
}

void ADeepSeekR1AIInterface::ProcessAIResponse(const FString& Response)
{
    // 解析JSON响应
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        FString AIResponse;
        if (JsonObject->TryGetStringField(TEXT("response"), AIResponse))
        {
            OnAIResponseReceived.Broadcast(AIResponse);
        }
        else
        {
            OnAIResponseReceived.Broadcast(TEXT("Failed to parse AI response"));
        }
    }
    else
    {
        OnAIResponseReceived.Broadcast(TEXT("Invalid response format"));
    }
}

FString ADeepSeekR1AIInterface::GetSceneContext(AActor* ContextActor)
{
    FString Context;
    
    if (ContextActor)
    {
        // 获取Actor的基本信息
        Context += FString::Printf(TEXT("Actor Name: %s\n"), *ContextActor->GetName());
        Context += FString::Printf(TEXT("Location: X=%f, Y=%f, Z=%f\n"), 
            ContextActor->GetActorLocation().X,
            ContextActor->GetActorLocation().Y,
            ContextActor->GetActorLocation().Z);
        Context += FString::Printf(TEXT("Rotation: Pitch=%f, Yaw=%f, Roll=%f\n"),
            ContextActor->GetActorRotation().Pitch,
            ContextActor->GetActorRotation().Yaw,
            ContextActor->GetActorRotation().Roll);

        // 获取组件信息
        TArray<UActorComponent*> Components;
        ContextActor->GetComponents(Components);
        Context += TEXT("\nComponents:\n");
        for (UActorComponent* Component : Components)
        {
            Context += FString::Printf(TEXT("- %s\n"), *Component->GetName());
        }
    }
    else
    {
        // 获取整个场景信息
        UWorld* World = GetWorld();
        if (World)
        {
            TArray<AActor*> AllActors;
            UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), AllActors);
            
            Context += TEXT("Scene Overview:\n");
            for (AActor* Actor : AllActors)
            {
                Context += FString::Printf(TEXT("- %s at Location: X=%f, Y=%f, Z=%f\n"),
                    *Actor->GetName(),
                    Actor->GetActorLocation().X,
                    Actor->GetActorLocation().Y,
                    Actor->GetActorLocation().Z);
            }
        }
    }

    return Context;
}

void ADeepSeekR1AIInterface::OnInteractionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, 
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
    bool bFromSweep, const FHitResult& SweepResult)
{
    // 检查是否是玩家角色
    if (OtherActor && OtherActor->IsA(AMyTPPProjectCharacter::StaticClass()))
    {
        bIsPlayerInRange = true;
        ShowInteractionWidget();
    }
}

void ADeepSeekR1AIInterface::OnInteractionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, 
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // 检查是否是玩家角色
    if (OtherActor && OtherActor->IsA(AMyTPPProjectCharacter::StaticClass()))
    {
        bIsPlayerInRange = false;
        HideInteractionWidget();
    }
}

void ADeepSeekR1AIInterface::ShowInteractionWidget()
{
    if (InteractionWidget)
    {
        InteractionWidget->SetVisibility(true);
    }
}

void ADeepSeekR1AIInterface::HideInteractionWidget()
{
    if (InteractionWidget)
    {
        InteractionWidget->SetVisibility(false);
    }
}
