// Copyright Michael Greenard. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

// Utility
#include "Library/AQSLibrary.h"

// Forward declarations
class APlayerController;
class UAQSSpawnComp;

#include "AQSGameMode.generated.h"

UCLASS()
class AQS_API AAQSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AAQSGameMode();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;


	/// FUNCTIONS ///

	// Use AQS to query a spawn location. Call when player has died and game is ready to find a respawn location.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (category = "GameMode|AQS"))
	void TryRespawnPlayer(ESpawnLocationType SpawnType, FVector DesiredLocation, AActor* DesiredAttachActor, APlayerController* Controller);

	// When a valid location has been found by AQS and player/game is ready to respawn the player, call this.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (category = "GameMode|AQS"))
	void RespawnPlayer(APlayerController* Controller, FVector& IdealLocation);

public:

	/// VARIABLES ///

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (category = "Components"))
	UAQSSpawnComp* AQSComponent;
};

