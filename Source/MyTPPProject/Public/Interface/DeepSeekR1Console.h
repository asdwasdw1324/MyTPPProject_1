// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeepSeekR1Console.generated.h"

UCLASS()
class ADeepSeekR1Console : public AActor
{
	GENERATED_BODY()

public:
	ADeepSeekR1Console();

	// Console command to ask questions
	UFUNCTION(Exec)
	void AskDeepSeek(const FString& Question);

	// Console command to analyze current scene or actor
	UFUNCTION(Exec)
	void AnalyzeSceneDeepSeek();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class ADeepSeekR1AIInterface* AIInterface;

	// Callback for AI responses
	UFUNCTION()
	void OnAIResponse(const FString& Response);
};
