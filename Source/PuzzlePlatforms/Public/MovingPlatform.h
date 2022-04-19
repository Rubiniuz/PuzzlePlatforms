#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	AMovingPlatform();
	~AMovingPlatform();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	bool bShouldReturn = true;

	UPROPERTY(EditAnywhere)
	bool bShouldLoop = false;
	
	UPROPERTY(EditAnywhere)
	float MoveSpeed = 20.0f;

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true)) // Metadata adds the widgetGizmo
	TArray<FVector> TargetLocations = TArray<FVector>();

	UPROPERTY(EditAnywhere)
	int LocationIndex = 0;

private:
	FVector GlobalTarget;
	FVector GlobalStartLocation;
	FTransform Original;
	int LocationIncrement = 1;
};
