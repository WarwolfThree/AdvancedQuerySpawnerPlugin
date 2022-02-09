// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvSpawnerPlayerState.h"

void AEnvSpawnerPlayerState::BeginPlay()
{
	Super::BeginPlay();

	SetTeamTag("Friendly"); // Set all pawns with a player state as friendly.
}