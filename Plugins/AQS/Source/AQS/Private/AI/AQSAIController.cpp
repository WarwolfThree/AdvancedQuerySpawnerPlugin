// Copyright Michael Greenard. All Rights Reserved.


#include "AI/AQSAIController.h"

void AAQSAIController::RunQuerySystem()
{
	UE_LOG(LogTemp, Display, TEXT("AQS - Running behaviour tree..."));
	if (GEngine && bPrintDebugString)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("AQS - Running behaviour tree..."));
	this->RunBehaviorTree(BehaviourTree);
}