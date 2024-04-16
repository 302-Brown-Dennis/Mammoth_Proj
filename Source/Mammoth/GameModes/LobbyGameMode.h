// Based on Stephen Ulibarri Udemy course https://www.udemy.com/course/unreal-engine-5-cpp-multiplayer-shooter/
// Modified by: Dennis Brown

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"
/**
 * 
 */

UCLASS()
class MAMMOTH_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	ALobbyGameMode();
	virtual void Tick(float DeltaTime) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelName")
	FString LevelName;
	
	void CheckPlayersReady();
	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 30.f;

	UPROPERTY(EditDefaultsOnly)
	float WarmUpTime = 5.f;

	float LevelStartingTime = 0.f;

private:

	int32 NumOfPlayers;

	float CountdownTime = 0.f;

	UPROPERTY()
	class ALobbyGameMode* LobbyGameMode;

	void CallServerTravel();
protected:

};
