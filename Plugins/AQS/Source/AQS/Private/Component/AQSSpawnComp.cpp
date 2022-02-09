// Copyright Michael Greenard. All Rights Reserved.


#include "Component/AQSSpawnComp.h"

// Utilities
#include "Kismet/KismetSystemLibrary.h"

// Other
#include "AI/AQSAIController.h"
#include "AI/AQSPawn.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Pawn.h"

// Sets default values for this component's properties
UAQSSpawnComp::UAQSSpawnComp() : 
		AQSPawnClass (AAQSPawn::StaticClass())
	,	AQSControllerClass (AAQSAIController::StaticClass())
	,	bPrintDebugString (false)
{

	PrimaryComponentTick.bCanEverTick = false;

	bAutoActivate = true;

}


// Called when the game starts
void UAQSSpawnComp::BeginPlay()
{
	Super::BeginPlay();

	// Leave these lines until last so we know when we are done initialising everything.
	UE_LOG(LogTemp, Display, TEXT("AQS - Spawn component has finished initialising."));
	if (GEngine && bPrintDebugString)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("AQS - Spawn component has finished initialising."));
	
}


// Called every frame
void UAQSSpawnComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

AAQSPawn* UAQSSpawnComp::CreateAQSPawn_Implementation(FVector DesiredLocation, APawn* OwningPawn, ESpawnLocationType SpawnLocationType)
{
	ensureAlways(IsValid(OwningPawn));

	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AAQSPawn* QueryPawn = GetWorld()->SpawnActor<AAQSPawn>(AQSPawnClass, DesiredLocation, Rotation, SpawnInfo);

	ensureAlways(IsValid(QueryPawn));

	QueryPawn->SetOwningPawn(OwningPawn);

	// Set the controller for the pawn to our desired controller.
	AController* OldController = QueryPawn->GetController();
	AAQSAIController* NewController = GetWorld()->SpawnActorDeferred<AAQSAIController>(AQSControllerClass, QueryPawn->GetTransform());
	ensureAlways(IsValid(NewController));
	NewController->BehaviourTree = AQSBehaviourTree;
	NewController->FinishSpawning(QueryPawn->GetTransform());

	NewController->Possess(QueryPawn);
	if (IsValid(OldController))
		OldController->Destroy();

	return QueryPawn;
}

void UAQSSpawnComp::QuerySpawnLocation_Implementation(APlayerController* PlayerController, ESpawnLocationType SpawnType, FVector DesiredLocation, AActor* DesiredAttachActor, AAQSPawn* AIPawn)
{
	ensureAlways(UKismetSystemLibrary::IsValidClass(AQSPawnClass) && UKismetSystemLibrary::IsValidClass(AQSControllerClass));

	switch (SpawnType)
	{
	case ESpawnLocationType::Location:
		FindIdealLocation(DesiredLocation, PlayerController, AIPawn);
		break;

	case ESpawnLocationType::PlayerStart:
		FindIdealPSLocation(DesiredLocation, PlayerController, AIPawn, DesiredAttachActor);
		break;

	case ESpawnLocationType::Player:
		FindIdealPlayerLocation(DesiredLocation, PlayerController, AIPawn, DesiredAttachActor);
		break;

	default: break;
	}

	return;

}

void UAQSSpawnComp::FindIdealLocation(FVector& Location, APlayerController*& Controller, AAQSPawn* AIPawn)
{
	UE_LOG(LogTemp, Display, TEXT("AQS - Finding ideal location..."));
	if (GEngine && bPrintDebugString)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("AQS - Finding ideal location..."));

	ensureAlways(IsValid(AIPawn));
	FDetachmentTransformRules DetachRules = FDetachmentTransformRules::KeepWorldTransform;
	AIPawn->DetachFromActor(DetachRules); // Detach from any actors we might have attached to from a previous query that wasn't strictly for a static location.
	ensureAlways(IsValid(Controller));
	APawn* PlayerPawn = Controller->GetPawn();
	
	FCollisionQueryParams RV_TraceParams;
	RV_TraceParams.bReturnPhysicalMaterial = false;
	//RV_TraceParams.AddIgnoredActor();

	// Trace down from location to hit floor
	FHitResult RV_Hit(ForceInit);

	GetWorld()->LineTraceSingleByChannel(
		RV_Hit,	//result
		Location,	//start
		FVector(Location.X, Location.Y, Location.Z - 1000),	//end
		ECC_Visibility,	//collision channel
		RV_TraceParams
	);

	if (RV_Hit.bBlockingHit)
		Location = RV_Hit.ImpactPoint;

	AIPawn->SetActorLocation(Location);

	AAQSAIController* AQSController = Cast<AAQSAIController>(AIPawn->GetController());
	ensureAlways(IsValid(AQSController));

	if(bPrintDebugString)
		AQSController->PrintMessages();

	if (IsValid(Controller) && IsValid(Controller->GetPawn()->GetPlayerState()))
	{
		if (Controller->GetPawn()->GetPlayerState()->GetClass()->ImplementsInterface(UAQSInterface::StaticClass()))
		{
			AQSController->RunQuerySystem();
		}
		else
		{
			AIPawn->Destroy();
			AQSController->Destroy();


			UE_LOG(LogTemp, Warning, TEXT("AQS - AQSPawn used doesn't implement interface. Cancelling query..."));
			if (GEngine && bPrintDebugString)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AQS - AQSPawn used doesn't implement interface. Cancelling query..."));
		}
	}
	else
	{
		AIPawn->Destroy();
		AQSController->Destroy();

		UE_LOG(LogTemp, Warning, TEXT("AQS - No valid controller given. Cancelling query..."));
		if (GEngine && bPrintDebugString)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AQS - No valid controller given. Cancelling query..."));
	}
}

void UAQSSpawnComp::FindIdealPSLocation(FVector& Location, APlayerController*& Controller, AAQSPawn* AIPawn, AActor* DesiredAttachActor)
{
	UE_LOG(LogTemp, Display, TEXT("AQS - Finding ideal location around player start..."));
	if (GEngine && bPrintDebugString)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("AQS - Finding ideal location around player start..."));

	ensureAlways(IsValid(AIPawn));
	FDetachmentTransformRules DetachRules = FDetachmentTransformRules::KeepWorldTransform;
	AIPawn->DetachFromActor(DetachRules); // Detach from any actors we might have attached to from a previous query that wasn't strictly for a static location.
	ensureAlways(IsValid(DesiredAttachActor));
	FAttachmentTransformRules AttachRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
	AIPawn->AttachToActor(DesiredAttachActor, AttachRules);
	ensureAlways(IsValid(Controller));
	APawn* PlayerPawn = Controller->GetPawn();

	FCollisionQueryParams RV_TraceParams;
	RV_TraceParams.bReturnPhysicalMaterial = false;
	RV_TraceParams.AddIgnoredActor(DesiredAttachActor);

	// Trace down from location to hit floor
	FHitResult RV_Hit(ForceInit);

	GetWorld()->LineTraceSingleByChannel(
		RV_Hit,	//result
		Location,	//start
		FVector(Location.X, Location.Y, Location.Z - 1000),	//end
		ECC_Visibility,	//collision channel
		RV_TraceParams
	);

	if (RV_Hit.bBlockingHit)
		Location = RV_Hit.ImpactPoint;

	AIPawn->SetActorLocation(Location);

	AAQSAIController* AQSController = Cast<AAQSAIController>(AIPawn->GetController());
	ensureAlways(IsValid(AQSController));

	if (bPrintDebugString)
		AQSController->PrintMessages();

	if (IsValid(Controller) && IsValid(Controller->GetPawn()->GetPlayerState()))
	{
		if (Controller->GetPawn()->GetPlayerState()->GetClass()->ImplementsInterface(UAQSInterface::StaticClass()))
		{
			AQSController->RunQuerySystem();
		}
		else
		{
			AIPawn->Destroy();
			AQSController->Destroy();


			UE_LOG(LogTemp, Warning, TEXT("AQS - AQSPawn used doesn't implement interface. Cancelling query..."));
			if (GEngine && bPrintDebugString)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AQS - AQSPawn used doesn't implement interface. Cancelling query..."));
		}
	}
	else
	{
		AIPawn->Destroy();
		AQSController->Destroy();

		UE_LOG(LogTemp, Warning, TEXT("AQS - No valid controller given. Cancelling query..."));
		if (GEngine && bPrintDebugString)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AQS - No valid controller given. Cancelling query..."));
	}
}

void UAQSSpawnComp::FindIdealPlayerLocation(FVector& Location, APlayerController*& Controller, AAQSPawn* AIPawn, AActor* DesiredAttachActor)
{
	UE_LOG(LogTemp, Display, TEXT("AQS - Finding ideal location around attach player..."));
	if (GEngine && bPrintDebugString)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("AQS - Finding ideal location around attach player..."));

	ensureAlways(IsValid(AIPawn));
	FDetachmentTransformRules DetachRules = FDetachmentTransformRules::KeepWorldTransform;
	AIPawn->DetachFromActor(DetachRules); // Detach from any actors we might have attached to from a previous query that wasn't strictly for a static location.
	ensureAlways(IsValid(DesiredAttachActor));
	FAttachmentTransformRules AttachRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
	AIPawn->AttachToActor(DesiredAttachActor, AttachRules);
	ensureAlways(IsValid(Controller));
	APawn* PlayerPawn = Controller->GetPawn();

	FCollisionQueryParams RV_TraceParams;
	RV_TraceParams.bReturnPhysicalMaterial = false;
	RV_TraceParams.AddIgnoredActor(DesiredAttachActor);

	// Trace down from location to hit floor
	FHitResult RV_Hit(ForceInit);

	GetWorld()->LineTraceSingleByChannel(
		RV_Hit,	//result
		Location,	//start
		FVector(Location.X, Location.Y, Location.Z - 1000),	//end
		ECC_Visibility,	//collision channel
		RV_TraceParams
	);

	if (RV_Hit.bBlockingHit)
		Location = RV_Hit.ImpactPoint;

	AIPawn->SetActorLocation(Location);

	AAQSAIController* AQSController = Cast<AAQSAIController>(AIPawn->GetController());
	ensureAlways(IsValid(AQSController));

	if (bPrintDebugString)
		AQSController->PrintMessages();

	if (IsValid(Controller) && IsValid(Controller->GetPawn()->GetPlayerState()))
	{
		if (Controller->GetPawn()->GetPlayerState()->GetClass()->ImplementsInterface(UAQSInterface::StaticClass()))
		{
			AQSController->RunQuerySystem();
		}
		else
		{
			AIPawn->Destroy();
			AQSController->Destroy();


			UE_LOG(LogTemp, Warning, TEXT("AQS - AQSPawn used doesn't implement interface. Cancelling query..."));
			if (GEngine && bPrintDebugString)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AQS - AQSPawn used doesn't implement interface. Cancelling query..."));
		}
	}
	else
	{
		AIPawn->Destroy();
		AQSController->Destroy();

		UE_LOG(LogTemp, Warning, TEXT("AQS - No valid controller given. Cancelling query..."));
		if (GEngine && bPrintDebugString)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AQS - No valid controller given. Cancelling query..."));
	}
}

