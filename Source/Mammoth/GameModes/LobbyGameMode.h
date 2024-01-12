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

	virtual void PostLogin(APlayerController* NewPlayer) override;

	
	void CheckPlayersReady();


private:

	int32 NumOfPlayers;

	UPROPERTY()
	class ALobbyGameMode* LobbyGameMode;

	void CallServerTravel();
protected:

};
