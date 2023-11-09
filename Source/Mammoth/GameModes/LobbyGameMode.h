// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAllPlayersReadyDelegate);

/**
 * 
 */
UCLASS()
class MAMMOTH_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "GameTravel")
	void TravelToNewLevel();
	
	ALobbyGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	void AllPlayersAcceptedTravel();

private:
	TArray<bool> PlayerInputStatus;
	int32 NumPlayersReady;

	void CheckAllPlayerInput();

	
};
