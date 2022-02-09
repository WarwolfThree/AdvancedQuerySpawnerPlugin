// Copyright Michael Greenard. All Rights Reserved.


#include "Game/AQSGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Component/AQSSpawnComp.h"
#include "GameFramework/PlayerState.h"
#include "AI/AQSPawn.h"
#include "Game/AQSPlayerState.h"

AAQSGameMode::AAQSGameMode() : Super()
{
	PlayerStateClass = AAQSPlayerState::StaticClass();

	// Create AQS component
	AQSComponent = this->CreateDefaultSubobject<UAQSSpawnComp>(TEXT("AQS Spawn Component"));

}

void AAQSGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (IsValid(NewPlayer->GetPawn()))
	{
		if (IsValid(NewPlayer->GetPawn()->GetPlayerState()))
		{
			AAQSPlayerState* PS = Cast<AAQSPlayerState>(NewPlayer->GetPawn()->GetPlayerState());
			ensureAlways(IsValid(PS));

			PS->SetTeamTag(TEXT("Friendly")); // Every user that logs into the server is treated as friendly.
		}
	}
}

void AAQSGameMode::TryRespawnPlayer_Implementation(ESpawnLocationType SpawnType, FVector DesiredLocation, AActor* DesiredAttachActor, APlayerController* Controller)
{
	ensureAlways(IsValid(Controller));

	APawn* OwningPawn = Controller->GetPawn();
	ensureAlways(IsValid(OwningPawn));

	AAQSPawn* AQSPawn = AQSComponent->CreateAQSPawn(DesiredLocation, OwningPawn, SpawnType);
	if (IsValid(Controller->GetPawn()->GetPlayerState()) && IsValid(AQSPawn))
	{
		if (Controller->GetPawn()->GetPlayerState()->GetClass()->ImplementsInterface(UAQSInterface::StaticClass()))
		{
			UE_LOG(LogTemp, Display, TEXT("Game Mode - AQS - Trying to respawn player..."));

			AAQSPlayerState* PS = Cast<AAQSPlayerState>(Controller->GetPawn()->GetPlayerState());
			ensureAlways(PS);
			PS->WantsRespawn();
			IAQSInterface::Execute_SetAQSPawnRef(PS, AQSPawn);
			AQSComponent->QuerySpawnLocation(Controller, SpawnType, DesiredLocation, DesiredAttachActor, AQSPawn);
		}
	}
}

void AAQSGameMode::RespawnPlayer_Implementation(APlayerController* Controller, FVector& IdealLocation)
{
	ensureAlways(IsValid(Controller));
	APawn* Pawn = Controller->GetPawn();
	ensureAlways(IsValid(Pawn));
	AAQSPlayerState* PS = Cast<AAQSPlayerState>(Pawn->GetPlayerState());
	ensureAlways(IsValid(PS));

	IAQSInterface::Execute_GetAQSPawnRef(PS)->GetController()->Destroy();
	IAQSInterface::Execute_GetAQSPawnRef(PS)->Destroy();

	UE_LOG(LogTemp, Display, TEXT("AQS - Player ready to be respawned"));
}
