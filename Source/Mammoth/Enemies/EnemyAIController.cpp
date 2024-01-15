// Enemy AI controller
// Author: Dennis Brown

#include "EnemyAIController.h"
#include "EnemyAI.h"
#include "Net/UnrealNetwork.h"
#include "Engine/NetConnection.h"
#include "Components/WidgetComponent.h"


void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}
AEnemyAIController::AEnemyAIController()
{
	bReplicates = true;
}

