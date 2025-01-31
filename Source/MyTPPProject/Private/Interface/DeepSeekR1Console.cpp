#include "Interface/DeepSeekR1Console.h"
#include "Interface/DeepSeekR1AIInterface.h"

ADeepSeekR1Console::ADeepSeekR1Console()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADeepSeekR1Console::BeginPlay()
{
	Super::BeginPlay();

	// 创建AI接口实例
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	AIInterface = GetWorld()->SpawnActor<ADeepSeekR1AIInterface>(ADeepSeekR1AIInterface::StaticClass(), SpawnParams);

	// 绑定回调
	if (AIInterface)
	{
		AIInterface->OnAIResponseReceived.AddDynamic(this, &ADeepSeekR1Console::OnAIResponse);
	}
}

void ADeepSeekR1Console::AskDeepSeek(const FString& Question)
{
	if (AIInterface)
	{
		AIInterface->AskQuestion(Question);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DeepSeek AI Interface not initialized"));
	}
}

void ADeepSeekR1Console::AnalyzeSceneDeepSeek()
{
	if (AIInterface)
	{
		AIInterface->AnalyzeScene();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DeepSeek AI Interface not initialized"));
	}
}

void ADeepSeekR1Console::OnAIResponse(const FString& Response)
{
	// 在控制台输出AI响应
	UE_LOG(LogTemp, Log, TEXT("DeepSeek AI Response: %s"), *Response);
    
	// 在屏幕上显示消息
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Response);
	}
}