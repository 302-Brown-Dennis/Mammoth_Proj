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

	UFUNCTION(BlueprintCallable, Category = PlayersHealth)
	float GetPlayerHealth();

private:
	UPROPERTY()
	class APlayerCharacter_cpp* Character;

	UPROPERTY()
	class AMammothPlayerController* MammothPlayerController;
	
};