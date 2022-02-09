// Copyright Michael Greenard. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AQSLibrary.generated.h"

/// ENUM ///

UENUM(BlueprintType)
enum ESpawnLocationType
{
	Location			UMETA(DisplayName = "Location"),
	PlayerStart			UMETA(DisplayName = "PlayerStart"),
	Player				UMETA(DisplayName = "Player"),
};

UCLASS()
class AQS_API UAQSLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
