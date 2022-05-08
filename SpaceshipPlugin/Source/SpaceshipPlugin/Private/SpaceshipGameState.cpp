// Copyright Pedro Matos. All Rights Reserved

#include "SpaceshipPlugin/Public/SpaceshipGameState.h"

// Sets default values
ASpaceshipGameState::ASpaceshipGameState()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpaceshipGameState::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpaceshipGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
