// Copyright Pedro Matos. All Rights Reserved


#include "SpaceshipPlayerController.h"


// Sets default values
ASpaceshipPlayerController::ASpaceshipPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpaceshipPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpaceshipPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

