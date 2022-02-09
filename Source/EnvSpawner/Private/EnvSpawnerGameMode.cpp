// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvSpawnerGameMode.h"
#include "EnvSpawnerCharacter.h"
#include "Game/AQSPlayerState.h"

void AEnvSpawnerGameMode::RespawnPlayer_Implementation(APlayerController* Controller, FVector& IdealLocation)
{
	Super::RespawnPlayer_Implementation(Controller, IdealLocation);

	ensure(IsValid(Controller));
	APawn* Pawn = Controller->GetPawn();
	ensure(IsValid(Pawn));
	AAQSPlayerState* PS = Cast<AAQSPlayerState>(Pawn->GetPlayerState());
	ensure(IsValid(PS));

	Pawn->SetActorLocation(IdealLocation);
	Pawn->SetActorRotation(FRotator(0.f, 0.f, FMath::RandRange(0.f, 360.f)));

	AEnvSpawnerCharacter* CharacterToRespawn = Cast<AEnvSpawnerCharacter>(Pawn);

	CharacterToRespawn->PlayerRespawn();

	UE_LOG(LogTemp, Display, TEXT("Game Mode - AQS - Player respawned"));
}