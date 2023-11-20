// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAllPlayersReadyDelegate);

UCLASS()
class MAMMOTH_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	ALobbyGameMode();

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	FAllPlayersReadyDelegate AllPlayersReadyDelegate;

private:
	TArray<bool> PlayerInputStatus;
	int32 NumPlayersReady;

	void CheckAllPlayersInput();
	void CallServerTravel();
protected:

};
