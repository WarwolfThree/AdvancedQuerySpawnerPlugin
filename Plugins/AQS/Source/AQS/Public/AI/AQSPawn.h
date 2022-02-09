// Copyright Michael Greenard. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "AQSPawn.generated.h"

UCLASS()
class AQS_API AAQSPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAQSPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	/// FUNCTIONS ///

	// Get all actors without our team tag. If no tag is given, this will assume all other pawns are enemies.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (category = "AQS"))
	TArray<APawn*> FindEnemyActors(FName FriendlyTeamTag);

	// Get all actors with our team tag. If no tag is given, this will assume all other pawns are enemies.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (category = "AQS"))
	TArray<APawn*> FindFriendlyActors(FName FriendlyTeamTag);

	// Set our owning player
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (category = "AQS"))
	void SetOwningPawn(APawn* NewPawn);

	// Get our owning player
	UFUNCTION(BlueprintPure, BlueprintCallable, meta = (category = "AQS"))
	APawn* GetOwningPawn();

	// Set our found location
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (category = "AQS"))
	void SetFoundLocation(FVector NewLocation);



	/// VARIABLES ///

public:
	// Have we found a suitable spawn location?
	UPROPERTY(Replicated, BlueprintReadOnly, meta = (category = "AQS"))
	bool bFoundLocation;

	// Our suitable spawn location. Only try to spawn here if bFoundLocation == true.
	UPROPERTY(BlueprintReadOnly, meta = (category = "AQS"))
	FVector FoundLocation;

private:

	APawn* OwningPawn;

};

/// INLINE ///

FORCEINLINE void AAQSPawn::SetFoundLocation_Implementation(FVector NewLocation) { FoundLocation = NewLocation; bFoundLocation = true; }