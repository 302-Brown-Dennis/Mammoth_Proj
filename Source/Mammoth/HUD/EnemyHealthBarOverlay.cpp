// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHealthBarOverlay.h"
#include "Components/ProgressBar.h"

float UEnemyHealthBarOverlay::UpdateEnemyHealthBar(float Percent)
{
	
	EnemyHealthBar->SetPercent(Percent);

	return Percent;
}

void UEnemyHealthBarOverlay::EnableVisibilty()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		class APlayerController* PlayerController = It->Get();
		SetVisibility(ESlateVisibility::Visible);
	}
	
}
