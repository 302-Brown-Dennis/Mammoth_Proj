// Each players state
// Author: Dennis Brown

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MammothPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API AMammothPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	AMammothPlayerState();

	
	void SetPlayerIsReady();
	bool GetPlayerIsReady() const;

	UFUNCTION(Server, Reliable)
	void Server_SetPlayerRead(bool bNewReadyState);

	bool bPlayerReady = false;

private:
	UPROPERTY()
	class APlayerCharacter_cpp* Character;

	
	
};