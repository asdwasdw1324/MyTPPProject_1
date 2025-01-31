// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DeepSeekR1Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDeepSeekR1Interface : public UInterface
{
    GENERATED_BODY()
};

/**
 * Interface for DeepSeek R1 AI functionality
 */
class IDeepSeekR1Interface
{
    GENERATED_BODY()

public:
    // 定义接口方法
    virtual void AskQuestion(const FString& Question, AActor* ContextActor = nullptr) = 0;
    virtual void AnalyzeScene(AActor* TargetActor = nullptr) = 0;
};
