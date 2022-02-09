// Copyright Michael Greenard. All Rights Reserved.


#include "Game/AQSPlayerState.h"


// Utilities
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "AI/AQSPawn.h"
#include "Game/AQSGameMode.h"
#include "Component/AQSSpawnComp.h"

// Sets default values
AAQSPlayerState::AAQSPlayerState()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;


}

void AAQSPlayerState::ChangeSpawnLocation(ESpawnLocationType SpawnType, FVector DesiredLocation, AActor* DesiredAttachActor, APlayerController* Controller)
{
	AAQSGameMode* GameMode = Cast<AAQSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	ensureAlways(IsValid(GameMode));
	ensureAlways(IsValid(AQSPawnRef));
	ensureAlways(IsValid(Controller));

	GameMode->AQSComponent->QuerySpawnLocation(Controller, SpawnType, DesiredLocation, DesiredAttachActor, AQSPawnRef);
}

void AAQSPlayerState::RespawnPlayer()
{
	if (bWaitingForRespawnLocation && GetNetMode() != ENetMode::NM_Client && IsValid(AQSPawnRef))
	{
		if (AQSPawnRef->bFoundLocation)
		{
			AAQSGameMode* GameMode = Cast<AAQSGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			APawn* OwnerPawn = Cast<APawn>(GetPawn());
			ensureAlways(IsValid(OwnerPawn));
			ensureAlways(IsValid(GameMode));
			APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController());
			ensureAlways(IsValid(PC));

			NoLongerWantsRespawn();
			GameMode->RespawnPlayer(PC, AQSPawnRef->FoundLocation);
		}
	}
}

void AAQSPlayerState::ClientRequestRespawnLocation_Implementation (ESpawnLocationType SpawnType, FVector DesiredLocation, AActor* DesiredAttachActor, APlayerController* Controller)
{
	ChangeSpawnLocation(SpawnType, DesiredLocation, DesiredAttachActor, Controller);
}

void AAQSPlayerState::ClientRequestRespawn_Implementation()
{
	RespawnPlayer();
}

bool AAQSPlayerState::ClientRequestRespawnLocation_Validate(ESpawnLocationType SpawnType, FVector DesiredLocation, AActor* DesiredAttachActor, APlayerController* Controller)
{
	return ValidateSpawnLocationRequest();
}

bool AAQSPlayerState::ClientRequestRespawn_Validate()
{
	return ValidateSpawnRequest();
}

bool AAQSPlayerState::ValidateSpawnLocationRequest()
{
	return true;
}

bool AAQSPlayerState::ValidateSpawnRequest()
{
	return true;
}

void AAQSPlayerState::SetAQSPawnRef_Implementation(AAQSPawn* InPawn) { ensure(IsValid(InPawn)); AQSPawnRef = InPawn; }

void AAQSPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAQSPlayerState, AQSPawnRef);
	DOREPLIFETIME(AAQSPlayerState, TeamTag);

}

