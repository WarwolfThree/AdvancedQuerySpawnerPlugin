// Copyright Michael Greenard. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

// Interfaces
#include "Interface/AQSInterface.h"

// Utilities
#include "Library/AQSLibrary.h"

// Forward declarations
class AAQSPawn;

#include "AQSPlayerState.generated.h"

UCLASS()
class AQS_API AAQSPlayerState : public APlayerState, public IAQSInterface
{
	GENERATED_BODY()

public:
	
	// Sets default values for this pawn's properties
	AAQSPlayerState();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	// Get team tag, primarily used for finding friendly squad/teammates to respawn on.
	UFUNCTION(BlueprintCallable, meta = (Category = "AQS"))
	FName GetTeamTag();

	// Set team tag, primarily used for finding friendly squad/teammates to respawn on.
	UFUNCTION(BlueprintCallable, meta = (Category = "AQS"))
	FName SetTeamTag(FName NewTeamTag);


	/// AQS FUNCTIONS ///
public:

	FName GetPlayerTeamTag_Implementation() override;
	AAQSPawn* GetAQSPawnRef_Implementation() override;

	/// Client only

	// Call when player is requesting a new respawn location if they're allowed to do so.
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, meta = (Category = "AQS"))
	void ClientRequestRespawnLocation(ESpawnLocationType SpawnType, FVector DesiredLocation, AActor* DesiredAttachActor, APlayerController* Controller);
	void ClientRequestRespawnLocation_Implementation(ESpawnLocationType SpawnType, FVector DesiredLocation, AActor* DesiredAttachActor, APlayerController* Controller);
	bool ClientRequestRespawnLocation_Validate(ESpawnLocationType SpawnType, FVector DesiredLocation, AActor* DesiredAttachActor, APlayerController* Controller);

	// Call when player has requested a respawn location. Only call this once a location has actually been found.
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, meta = (Category = "AQS"))
	void ClientRequestRespawn();
	void ClientRequestRespawn_Implementation();
	bool ClientRequestRespawn_Validate();


	/// Server only

	void SetAQSPawnRef_Implementation(AAQSPawn* InPawn) override;

	// Call when player no longer can respawn.
	UFUNCTION(BlueprintCallable, meta = (Category = "AQS"))
	void NoLongerWantsRespawn();

	// Call when player now has a valid location to spawn to.
	UFUNCTION(BlueprintCallable, meta = (Category = "AQS"))
	void WantsRespawn();

	// Request a new respawn location with given params.
	UFUNCTION(BlueprintCallable, meta = (Category = "AQS"))
	void ChangeSpawnLocation(ESpawnLocationType SpawnType, FVector DesiredLocation, AActor* DesiredAttachActor, APlayerController* Controller);

	// Call when respawning player. Override (but keep a super call first), to add any custom respawn logic.
	UFUNCTION(BlueprintCallable, meta = (Category = "AQS"))
	void RespawnPlayer();

	// Override this in child class. Verifies data being given from client to server for a change in spawn location is correct.
	UFUNCTION(BlueprintCallable, meta = (Category = "AQS"))
	bool ValidateSpawnLocationRequest();

	// Override this in child class. Verifies data being given from client to server for client side manual respawn is correct.
	UFUNCTION(BlueprintCallable, meta = (Category = "AQS"))
	bool ValidateSpawnRequest();




	/// AQS VARIABLES ///

protected:

	// Actor tag to look for when looking for friendly pawns.
	UPROPERTY(Replicated)
	FName TeamTag;

private:

	bool bWaitingForRespawnLocation;

protected:

	UPROPERTY(Replicated)
	AAQSPawn* AQSPawnRef;

};

/// INLINE ///

FORCEINLINE FName AAQSPlayerState::GetTeamTag() { return TeamTag; }
FORCEINLINE FName AAQSPlayerState::SetTeamTag(FName NewTeamTag) { TeamTag = NewTeamTag; return TeamTag; }
FORCEINLINE void AAQSPlayerState::NoLongerWantsRespawn() { bWaitingForRespawnLocation = false; }
FORCEINLINE void AAQSPlayerState::WantsRespawn() { bWaitingForRespawnLocation = true; }

FORCEINLINE FName AAQSPlayerState::GetPlayerTeamTag_Implementation() { return TeamTag; }
FORCEINLINE AAQSPawn* AAQSPlayerState::GetAQSPawnRef_Implementation() { return AQSPawnRef; }