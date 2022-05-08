// Copyright Pedro Matos. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GeneralMovement/Public/Framework/Actors/GenPlayerController.h"
#include "SpaceshipPlayerController.generated.h"

UCLASS(Blueprintable, BlueprintType)
class SPACESHIPPLUGIN_API ASpaceshipPlayerController : public AGenPlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpaceshipPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
