// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthBarOverlay.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API UEnemyHealthBarOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* EnemyHealthBar;
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	float UpdateEnemyHealthBar(float Percent);

	void EnableVisibilty();
};
