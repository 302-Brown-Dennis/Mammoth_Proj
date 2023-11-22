// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LevelsGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API ALevelsGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	UFUNCTION(Server, Reliable)
	void Server_GetServerTime();

private:
protected:


	
};
