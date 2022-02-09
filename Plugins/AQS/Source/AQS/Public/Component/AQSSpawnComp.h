// Copyright Michael Greenard. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Library/AQSLibrary.h"
#include "Interface/AQSInterface.h"

// Forward declarations
class AAQSPawn;
class AAQSAIController;
class UBehaviorTree;
class APawn;

#include "AQSSpawnComp.generated.h"

UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AQS_API UAQSSpawnComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAQSSpawnComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



	/// FUNCTION ///

	// Create an AQS pawn for query.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (category = "AQS"))
	AAQSPawn* CreateAQSPawn(FVector DesiredLocation, APawn* OwningPawn, ESpawnLocationType SpawnLocationType);

	// Try to find a valid location to spawn.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (category = "AQS"))
	void QuerySpawnLocation(APlayerController *PlayerController, ESpawnLocationType SpawnType, FVector DesiredLocation, AActor* DesiredAttachActor, AAQSPawn* AIPawn);



private:

	void FindIdealPlayerLocation(FVector& Location, APlayerController*& Controller, AAQSPawn* AIPawn, AActor* DesiredAttachActor);
	void FindIdealPSLocation(FVector& Location, APlayerController*& Controller, AAQSPawn* AIPawn, AActor* DesiredAttachActor);
	void FindIdealLocation(FVector& Location, APlayerController*& Controller, AAQSPawn* AIPawn);



	/// DEFAULTS ///

public:

	// Pawn class to use when querying spawn location. Used to run behaviour tree and EQS within the level.
	UPROPERTY(EditDefaultsOnly, meta = (category = "AQS"))
	TSubclassOf<class AAQSPawn> AQSPawnClass;

	// AI controller class to use when querying spawn location. This will be applied to the spawned AQS pawn.
	UPROPERTY(EditDefaultsOnly, meta = (category = "AQS"))
	TSubclassOf<class AAQSAIController> AQSControllerClass;

	// AI behaviour tree that AI controller will use
	UPROPERTY(EditDefaultsOnly, meta = (category = "AQS"))
	UBehaviorTree* AQSBehaviourTree;

	// Should component print strings on screen
	UPROPERTY(EditDefaultsOnly, meta = (category = "AQS"))
	bool bPrintDebugString;
		
};
