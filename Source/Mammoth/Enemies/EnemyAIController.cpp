// Enemy AI controller
// Author: Dennis Brown

#include "EnemyAIController.h"
#include "EnemyAI.h"
#include "Net/UnrealNetwork.h"
#include "Engine/NetConnection.h"
#include "Components/WidgetComponent.h"


void AEnemyAIController::BeginPlay()
{
	if (WidgetEnemyHealthBar)
	{
		//EnemyHealthBar = CreateWidget<UUserWidget>(this, WidgetEnemyHealthBar);
		if (EnemyHealthBar)
		{
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
AEnemyAIController::AEnemyAIController()
{
	bReplicates = true;
}

void AEnemyAIController::DisplayEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
	
}

void AEnemyAIController::RemoveEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}
