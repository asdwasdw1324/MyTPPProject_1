// #include "Interface/DeepSeekR1Console.h"
// #include "Interface/DeepSeekR1AIInterface.h"
//
// ADeepSeekR1Console::ADeepSeekR1Console()
// {
// 	PrimaryActorTick.bCanEverTick = false;
// }
//
// void ADeepSeekR1Console::BeginPlay()
// {
// 	Super::BeginPlay();
//
// 	// 创建AI接口实例
// 	FActorSpawnParameters SpawnParams;
// 	SpawnParams.Owner = this;
// 	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
// 	AIInterface = GetWorld()->SpawnActor<ADeepSeekR1AIInterface>(ADeepSeekR1AIInterface::StaticClass(), FTransform::Identity, SpawnParams);
//
// 	// 绑定回调
// 	if (!AIInterface)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn AI Interface!"));
// 	}
//
// 	if (!AIInterface->GetClass()->ImplementsInterface(UDeepSeekR1Interface::StaticClass()))
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("Spawned actor missing interface implementation"));
// 		AIInterface->Destroy();
// 		AIInterface = nullptr;
// 		return;
// 	}
//
// 	AIInterface->OnAIResponseReceived.AddDynamic(this, &ADeepSeekR1Console::OnAIResponse);
// 	
// }
//
// void ADeepSeekR1Console::AskDeepSeek(const FString& Question)
// {
// 	if (AIInterface)
// 	{
// 		AIInterface->AskQuestion(Question);
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("DeepSeek AI Interface not initialized"));
// 	}
// }
//
// void ADeepSeekR1Console::AnalyzeSceneDeepSeek()
// {
// 	if (AIInterface)
// 	{
// 		AIInterface->AnalyzeScene();
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("DeepSeek AI Interface not initialized"));
// 	}
// }
//
// void ADeepSeekR1Console::OnAIResponse(const FString& Response)
// {
//     AsyncTask(ENamedThreads::GameThread, [this, Response]()
//     {
//         UE_LOG(LogTemp, Log, TEXT("[DeepSeek] AI Response: %s"), *Response);
//         
//         if (GEngine)
//         {
//             GEngine->AddOnScreenDebugMessage(
//                 -1, 
//                 5.0f,
//                 FColor::Green,
//                 FString::Printf(TEXT("DeepSeek: %s"), *Response.Left(200)) // 限制显示长度
//             );
//         }
//     });
// }