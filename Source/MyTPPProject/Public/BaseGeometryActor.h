// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WuKongInterface_Prop.h"
#include "GameFramework/Actor.h"
#include "WuKongInterface_Prop.h"
#include "BaseGeometryActor.generated.h"

class UStaticMeshComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimerFinished, AActor*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnColorChanged, const FLinearColor&, Color, const FString&, Name);

//Enumeration of Movement Type
UENUM(BlueprintType)
enum class EMovementType : uint8
{
	Sin UMETA(DisplayName = "SinState"),
	Static UMETA(DisplayName = "SaticState")
};

UENUM(BlueprintType)
enum class EGeometryType : uint8
{
	GeoOption1 UMETA(DisplayName = "Opt1"),
	GeoOption2 UMETA(DisplayName = "Opt2"),
	GeoOption3 UMETA(DisplayName = "Opt3")
};

UENUM(BlueprintType)
namespace EMyEnumeration
{
	enum Type
	{
		Type1,
		Type2,
		Type3
	};
}

//Related properties of BaseGeometry actor packed in FGeometry struct
USTRUCT(BlueprintType)
struct FGeometryData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float GeoAmplitude = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float GeoFrequency = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementType GeoMoveType = EMovementType::Static;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EGeometryType GeoType = EGeometryType::GeoOption1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
	FLinearColor GeoColor = FLinearColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
	float ChangeColorTimeRate = 3.0f;
	
};

UCLASS()
class MYTPPPROJECT_API ABaseGeometryActor : public AActor, public IWuKongInterface_Prop
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseGeometryActor();

	//Define a GeometryData struct
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|GeoData")
	FGeometryData GeometryData;

	//Get and Set GeometryData
	UFUNCTION(BlueprintCallable)
	void SetGeometryData(const FGeometryData& Data)
	{
		GeometryData = Data;
	}

	UFUNCTION(BlueprintCallable)
	FGeometryData GetGeometryData() const
	{
		return GeometryData;
	}
	//Get and Set GeometryData

	//Delegate instance for ending to change color
	FOnTimerFinished OnTimerFinished;

	//Delegate instance for changing color
	UPROPERTY(BlueprintAssignable)
	FOnColorChanged OnColorChanged;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* BaseMesh;

	UFUNCTION(BlueprintCallable)
	void MoveLogic();

	UFUNCTION(BlueprintCallable)
	void SetColor(const FLinearColor& Color);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Store this actor's initial location when game starts
	FVector GeoInitialLocation;

	//TimerHandle for changing color
	FTimerHandle ColorChangeTimerHandle;

	//ColorChange parameters
	const int32 MaxTimerCount = 10;
	int32 TimerCount = 0;

	//Fire to change color function
	void OnTimeFire();

	////Delegate Test function
	UFUNCTION()
	void ColorChangeFunc(const FLinearColor& Color, const FString& Name);

	void TimerFinishedFunc(AActor* FinishedGeoActor);
	//Delegate Test function
	
	//Implementation function to heal character
	virtual void HealInteract_Implementation(APawn* InstigatorPawn);

	TEnumAsByte<EMyEnumeration::Type>MyEnum;
	
};
