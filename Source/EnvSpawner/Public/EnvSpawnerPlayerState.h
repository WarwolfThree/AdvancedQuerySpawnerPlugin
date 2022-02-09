// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/AQSPlayerState.h"
#include "EnvSpawnerPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ENVSPAWNER_API AEnvSpawnerPlayerState : public AAQSPlayerState
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	
};
