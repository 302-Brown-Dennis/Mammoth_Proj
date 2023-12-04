// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LevelsGameMode.generated.h"

namespace MatchState
{
	extern MAMMOTH_API const FName Cooldown; // level ended, show stats, start timer
}
UCLASS()
class MAMMOTH_API ALevelsGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	ALevelsGameMode();
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(Server, Reliable)
	void Server_GetServerTime();

	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 30.f;

	float LevelStartingTime = 0.f;

private:
	float CountdownTime = 0.f;

	

protected:

	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;


	
};
