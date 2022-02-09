// Copyright Michael Greenard. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

// Utilities
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/StreamableManager.h"
#include "Library/AQSLibrary.h"

// Components
#include "BehaviorTree/BehaviorTree.h"

#include "AQSAIController.generated.h"

/**
 * 
 */
UCLASS()
class AQS_API AAQSAIController : public AAIController
{
	GENERATED_BODY()

public:

	/// FUNCTIONS ///

	// Run AI behaviour and begin EQS
	UFUNCTION(BlueprintCallable, meta = (category = "AQS"))
	void RunQuerySystem();

	/// Getter/Setter ///

	// Getter for spawn type this AI is attempting to find.
	UFUNCTION(BlueprintPure, BlueprintCallable, meta = (category = "AQS"))
	ESpawnLocationType GetSpawnType();

	// Setter for spawn type this AI is attempting to find.
	UFUNCTION(BlueprintCallable, meta = (category = "AQS"))
	void SetSpawnType(ESpawnLocationType NewSpawnType);

	void PrintMessages();



	/// VARIABLES ///

private:

	bool bPrintDebugString;
	ESpawnLocationType SpawnType;

public:

	UBehaviorTree* BehaviourTree;
	
};

/// INLINE ///

FORCEINLINE void AAQSAIController::PrintMessages() { bPrintDebugString = true; }
FORCEINLINE ESpawnLocationType AAQSAIController::GetSpawnType() { return SpawnType; }
FORCEINLINE void AAQSAIController::SetSpawnType(ESpawnLocationType NewSpawnType) { SpawnType = NewSpawnType; }