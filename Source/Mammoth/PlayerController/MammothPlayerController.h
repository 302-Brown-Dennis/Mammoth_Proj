// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MammothPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API AMammothPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDStamina(float Stamina, float MaxStamina);
	
protected:
	virtual void BeginPlay() override;
	

private:
	class AMammothHUD* MammothHUD;
};
