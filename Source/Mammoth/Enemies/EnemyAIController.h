// Enemy AI controller
// Author: Dennis Brown

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	
protected:
	virtual void BeginPlay() override;
};
