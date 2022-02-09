// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/AQSGameMode.h"
#include "EnvSpawnerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ENVSPAWNER_API AEnvSpawnerGameMode : public AAQSGameMode
{
	GENERATED_BODY()

	void RespawnPlayer_Implementation(APlayerController* Controller, FVector& IdealLocation) override;
	
};
