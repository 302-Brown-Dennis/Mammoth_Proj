// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MammothGameState.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API AMammothGameState : public AGameState
{
	GENERATED_BODY()
	
public:

	AMammothGameState();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Mammoth|GameState")
	TArray<AMammothPlayerState*> GetPlayerArray() const;

	UFUNCTION(BlueprintCallable, Category = "Mammoth|GameState")
	int32 GetNumPlayers() const;

	UPROPERTY(Replicated)
	TArray<class AMammothPlayerState*> MissionReadyPlayers;

private:
	int32 NumOfPlayersReady;

	void GetRead();
};
