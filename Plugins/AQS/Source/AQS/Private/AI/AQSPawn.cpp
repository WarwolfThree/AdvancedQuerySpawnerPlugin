// Copyright Michael Greenard. All Rights Reserved.


#include "AI/AQSPawn.h"

// Utilities
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Interface
#include "Interface/AQSInterface.h"

#include "GameFramework/PlayerState.h"

// Sets default values
AAQSPawn::AAQSPawn() :
		bFoundLocation(false)
	,	FoundLocation(0,0,0)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set Up Replication
	bReplicates = true;

}

// Called when the game starts or when spawned
void AAQSPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAQSPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAQSPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAQSPawn::SetOwningPawn_Implementation(APawn* NewPawn)
{
	OwningPawn = NewPawn; ensureAlways(IsValid(OwningPawn));
}

APawn* AAQSPawn::GetOwningPawn()
{
	return OwningPawn;
}

TArray<APawn*> AAQSPawn::FindEnemyActors_Implementation(FName FriendlyTeamTag)
{
	TArray<AActor*> FoundActors;
	TArray<APawn*> OutActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), FoundActors);

	if (FoundActors.Contains(OwningPawn))
		FoundActors.Remove(OwningPawn); // Don't factor in the current player into the visibility check

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		APawn* CurrentPawn = Cast<APawn>(FoundActors[i]);
		if (IsValid(CurrentPawn->GetPlayerState()))
		{
			APlayerState* CurrentPS = CurrentPawn->GetPlayerState();
			if (FriendlyTeamTag == "" || FriendlyTeamTag == "None") // If we weren't given a team name, assume all other pawns are enemies
				OutActors.Add(CurrentPawn);
			else
			{
				if (CurrentPS->GetClass()->ImplementsInterface(UAQSInterface::StaticClass()))
				{
					if (IAQSInterface::Execute_GetPlayerTeamTag(CurrentPS) != FriendlyTeamTag)
						OutActors.Add(CurrentPawn);
				}
			}
		}
		else
		{
			// Consider pawns without a playerstate as enemies
			OutActors.Add(CurrentPawn);
		}
	}

	return OutActors;
}

TArray<APawn*> AAQSPawn::FindFriendlyActors_Implementation(FName FriendlyTeamTag)
{
	TArray<AActor*> FoundActors;
	TArray<APawn*> OutActors;

	if (FriendlyTeamTag == "" || FriendlyTeamTag == "None") // If we weren't given a team name, assume all other pawns are enemies
		return OutActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), FoundActors);

	if (FoundActors.Contains(OwningPawn))
		FoundActors.Remove(OwningPawn); // Don't factor in the current player into the visibility check

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		APawn* CurrentPawn = Cast<APawn>(FoundActors[i]);
		if (IsValid(CurrentPawn->GetPlayerState()))
		{
			APlayerState* CurrentPS = CurrentPawn->GetPlayerState();
				if (CurrentPS->GetClass()->ImplementsInterface(UAQSInterface::StaticClass()))
				{
					if (IAQSInterface::Execute_GetPlayerTeamTag(CurrentPS) == FriendlyTeamTag)
						OutActors.Add(CurrentPawn);
				}
		}
	}

	return OutActors;
}

void AAQSPawn::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAQSPawn, bFoundLocation);

}