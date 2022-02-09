// Copyright Michael Greenard. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Forward declations
class AAQSPawn;

#include "AQSInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAQSInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AQS_API IAQSInterface
{
	GENERATED_BODY()

public:

	/// FUNCTIONS ///

	// Get identifiable team tag from pawn.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (category = "AQS"))
	FName GetPlayerTeamTag();

	// Give player state a pointer to querying AI for replicated updates.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (category = "AQS"))
	void SetAQSPawnRef(AAQSPawn* InPawn);

	// Get AQS pawn ref from player state.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (category = "AQS"))
	AAQSPawn* GetAQSPawnRef();

};

