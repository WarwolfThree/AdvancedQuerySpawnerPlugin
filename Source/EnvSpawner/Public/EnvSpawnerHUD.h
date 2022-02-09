// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EnvSpawnerHUD.generated.h"

UCLASS()
class AEnvSpawnerHUD : public AHUD
{
	GENERATED_BODY()

public:
	AEnvSpawnerHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

